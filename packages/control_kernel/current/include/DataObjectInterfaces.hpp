#ifndef DATAOBJECTINTERFACES_HPP
#define DATAOBJECTINTERFACES_HPP


#include "ModuleControlInterface.hpp"
#include <corelib/Time.hpp>
#include <corelib/HeartBeatGenerator.hpp>
#include <corelib/Event.hpp>
#include <os/MutexLock.hpp>
#include <asm/atomic.h>
#undef LOCK

namespace ORO_ControlKernel
{

    /**
     * @brief A DataObjectInterface instance makes a data
     * struct available for the outside world to read from and write to.
     *
     * We don't use this class when efficiency is important 
     * (a virtual function can not be inlined), but it is handy for 
     * complexer DataObjects.
     *
     * @see DataObject
     */
    template <class _DataType>
    struct DataObjectInterface
    {
        /**
         * The type of the data.
         */
        typedef _DataType DataType;
            
        /**
         * Get a copy of the Data of the module.
         *
         * @param pull A copy of the data.
         */
        virtual void Get( DataType& pull ) const = 0;

        /**
         * Get a const reference of the data of the module.
         *
         * @return The result of the module.
         */
        virtual const DataType& Get() const = 0;

        /**
         * Set the data to a certain value.
         *
         * @param push The data which must be set.
         */
        virtual void Set( const DataType& push ) = 0;

    };


    /**
     * @brief A class which provides locked/protected access to one typed element of data.
     *
     * It allows multiple read/write requests using a single lock. This is the in any case
     * threadsafe implementation, and can be blocking in situations where you do not want
     * that. Use the DataObjectPrioritySet and DataObjectPriorityGet classes for non
     * blocking Set or Get operations.
     */
    template<class _DataType>
    class DataObjectLocked
    {
        mutable ORO_OS::Mutex lock;
            
        /**
         * One element of Data.
         */
        _DataType data;

        mutable _DataType cache;
    public:
        /**
         * The type of the data.
         */
        typedef _DataType DataType;
            
        /**
         * Get a copy of the Data of the module.
         *
         * @param pull A copy of the data.
         */
        void Get( DataType& pull ) const { ORO_OS::MutexLock locker(lock); pull = data; }

        /**
         * Get a const reference to a copy of the data of the module.
         * This method is thread-safe.
         *
         * @return The result of the module.
         */
        const DataType& Get() const { Get(cache); return cache; }

        /**
         * Set the data to a certain value.
         *
         * @param push The data which must be set.
         */
        void Set( const DataType& push ) { ORO_OS::MutexLock locker(lock); data = push; }
    };

    /**
     * @brief A DataObject which allows only the high priority thread to Set(),
     *        and both low and high priorities to Get().
     *
     * A class which provides locked/protected access to one typed element of data,
     * where the locking is optimised for Low priority thread reads and High priority
     * thread writes. The Set() operation is never blocking.
     *
     * @verbatim
     * The following Truth table applies when a Low Priority thread is
     * preempted by a High Priority thread :
     *
     *   L\H | Set | Get |
     *   Set | NA  | Blk |
     *   Get | Ok  | Blk |
     *
     * legend : L : Low Priority thread
     *          H : High Priority thread
     *          Blk: Blocks High Priority thread (bad!)
     *          NA : Not Allowed !
     * @endverbatim
     * If only one thread accesses the DataObject simultaneously (e.g. single
     * threaded systems), then the Get() and Set() methods will never block.
     *
     * @invar The Set() context has a strictly higher priority than the Get() context.
     * @invar Set() never blocks.
     */
    template<class _DataType>
    class DataObjectPrioritySet
    {
        mutable ORO_OS::Mutex lock;
        mutable bool dirty_flag;
            
        /**
         * One element of Data.
         */
        _DataType data;
        _DataType copy;

        mutable _DataType cache;
    public:
        /**
         * The type of the data.
         */
        typedef _DataType DataType;
            
        /**
         * Get a copy of the Data of the module.
         *
         * @param pull A copy of the data.
         */
        void Get( DataType& pull ) const
        { if (dirty_flag) pull = copy; else {ORO_OS::MutexLock locker(lock); pull = data;} }

        /**
         * @brief Get a const reference to a copy of the data of the module.
         *
         * This method is only thread-safe with respect to a Set().
         *
         * @return A reference to a copy of the data.
         */
        const DataType& Get() const { Get(cache); return cache; }

        /**
         * Set the data to a certain value.
         *
         * @param push The data which must be set.
         */
        void Set( const DataType& push ) 
        { 
            ORO_OS::MutexTryLock locker(lock); 
            if (locker.isSuccessful()) 
                {data = push; dirty_flag = false;} 
            else 
                {copy = push; dirty_flag = true;}
        }
    };

    /**
     * @brief A DataObject which allows only the high priority thread to Get(),
     *        and both low and high priorities to Set().
     *
     * A class which provides locked/protected access to one typed element of data,
     * where the locking is optimised for Low priority thread writes and High priority
     * thread reads. The Get() operation is always non blocking.
     *
     * @warning This DataObject can only be used in a two thread with distinct static
     * priority model
     *
     * @verbatim
     * The following Truth table applies when a Low Priority thread is
     * preempted by a High Priority thread :
     *
     *   L\H | Set | Get |
     *   Set | NA  | Ok  |
     *   Get | Blk | Ok  |
     *
     * legend : L : Low Priority thread
     *          H : High Priority thread
     *          Blk: Blocks High Priority thread (bad!)
     *          NA : Not allowed !
     * @endverbatim
     * If only one thread accesses the DataObject simultaneously (e.g. single
     * threaded systems), then the Get() and Set() methods will never block.
     *
     * @invar The Set() context has a strictly lower priority than the Get() context.
     * @invar Get() never blocks.
     */
    template<class _DataType>
    class DataObjectPriorityGet
    {
        mutable ORO_OS::Mutex lock;
            
        /**
         * One element of Data.
         */
        _DataType data;
        _DataType copy;

        mutable _DataType cache;
    public:
        /**
         * The type of the data.
         */
        typedef _DataType DataType;
            
        /**
         * Get a copy of the Data of the module.
         *
         * @param pull A copy of the data.
         */
        void Get( DataType& pull ) const 
        { 
            // If two low priority gets access this method,
            // while Set() is in copy, 
            // the second one will pull in the copy which
            // is modified by the Set().
            ORO_OS::MutexTryLock locker(lock); 
            if ( locker.isSuccessful() ) 
                {pull = data;} 
            else 
                {pull = copy;}
        }

        /**
         * Get a const reference to a copy of the data of the module.
         * This method is thread-safe with respect to Set().
         *
         * @return A reference to a copy of the data.
         */
        const DataType& Get() const { Get(cache); return cache; }

        /**
         * Set the data to a certain value.
         *
         * @param push The data which must be set.
         */
        void Set( const DataType& push ) 
        { 
            {
                ORO_OS::MutexLock locker(lock); 
                data = push; 
            }
            copy = data;
        }
    };

    /**
     * @brief This DataObject is implemented with a circular
     * buffer, such that reads and writes can happen concurrently.
     * 
     * When there are more writes than reads, the last write will
     * be returned. The internal buffer can get full if too many
     * concurrent reads are taking to long. In that case, each new
     * read will read the element the previous read returned.
     */
    template<class _DataType>
    class DataObjectBuffer
    {
        /**
         * Internal buffer structure.
         */
        struct DataBuf { _DataType data; mutable atomic_t counter; };

        DataBuf* read_ptr;
        DataBuf* write_ptr;
        const DataBuf* head;
        const DataBuf* tail;

        static const unsigned int BUF_LEN=10;
            
        /**
         * A 3 element Data buffer
         */
        DataBuf data[BUF_LEN];

        mutable _DataType cache;
    public:
        DataObjectBuffer() 
            : read_ptr(&data[ 0 ]), 
              write_ptr(&data[ 1 ]), 
              head(&data[ 0 ]), 
              tail(&data[ BUF_LEN-1 ]) {}
        
        /**
         * The type of the data.
         */
        typedef _DataType DataType;

        /**
         * Get a const reference to a copy of the data.
         * This method is not thread safe.
         *
         * @return A reference to a copy of the data.
         */
        const DataType& Get() const { Get(cache); return cache; }
            
        /**
         * Get a copy of the Data
         *
         * @param pull A copy of the data.
         */
        void Get( DataType& pull ) const 
        {   
            DataBuf* reading = read_ptr;        // atomic copy
            atomic_inc(reading->counter);       // atomic increment
            pull = reading->data;               // takes some time
            atomic_dec(reading->counter);       // atomic decrement
        }

        /**
         * Set the data to a certain value.
         *
         * @param push The data which must be set.
         */
        void Set( const DataType& push ) 
        {
            /**
             * This method can not be called concurrently (only one
             * producer). With a minimum of 3 buffers, if the 
             * write_ptr+1 field is not occupied, it will remain so
             * because the read_ptr is at write_ptr-1. Hence, no
             * locking is needed.
             */
            // writeout in any case
            write_ptr->data = push;
            // if next field is occupied, buffer is 'full'.
            if ( atomic_read( (write_ptr + 1)->counter ) != 0 )
                return;
            // we will be able to move, so replace read_ptr
            read_ptr = write_ptr;
            if ( write_ptr != tail)
                ++write_ptr;
            else 
                write_ptr = head;
        }
    };

    /**
     * @brief A class which provides unchecked access to one typed element of data.
     *
     * It is the most simple form of the DataObjectInterface implementation and
     * hence also the fastest. It is however not thread safe.
     */
    template<class _DataType>
    class DataObject
    {
        /**
         * One element of Data.
         */
        _DataType data;

    public:
        /**
         * The type of the data.
         */
        typedef _DataType DataType;
            
        /**
         * Get a copy of the Data of the module.
         *
         * @param pull A copy of the data.
         */
        void Get( DataType& pull ) const { pull = data; }

        /**
         * Get a const reference of the data of the module.
         *
         * @return The result of the module.
         */
        const DataType& Get() const { return data; }
            
        /**
         * Set the data to a certain value.
         *
         * @param push The data which must be set.
         */
        void Set( const DataType& push ) { data = push; }

    };

    /**
     * @brief A class which provides update based access to one typed element of data.
     */
    template<class _DataType>
    class DataObjectUpdated
    {
        /**
         * One element of Data.
         */
        _DataType data;
        mutable Event    updated;
    public:
        /**
         * The type of the data.
         */
        typedef _DataType DataType;
            
        /**
         * Get a copy of the Data of the module.
         *
         * @param pull A copy of the data.
         */
        void Get( DataType& pull ) const { pull = data; }

        /**
         * Get a const reference of the data of the module.
         *
         * @return The result of the module.
         */
        const DataType& Get() const { return data; }
            
        /**
         * Set the data to a certain value.
         *
         * @param push The data which must be set.
         */
        void Set( const DataType& push ) { data = push; updated.fire(); }

        /**
         * Allows you to register a Listenener,Completer handler
         * to be notified when the DataObject is updated.
         */
        ORO_CoreLib::HandlerRegistrationInterface* eventGet() { return &updated; }
    };

    /**
     * @brief This DataObject updates the data with each Get() method call.
     * @see DataObject
     * @see DataObjectInterface
     */
    template<class _DataType>
    class DataRecursive
        :public DataObject<_DataType>
    {
        /**
         * One element of Data.
         */
        _DataType data;
    public:
        typedef _DataType DataType;
            
        /**
         * Create a recursive data element which fetches the data from a
         * ModuleControlInterface.
         *
         * @param _module The module to provide the data for this instance.
         */
        DataRecursive( ModuleControlInterface* _module) : module(_module) {}
            
        void Get( DataType& pull ) const { module->update(); pull = data; }
        const DataType& Get() const { module->update(); return data; }
        void Set( const DataType& push ) { data = push; }
    protected:
        mutable ModuleControlInterface* module;
    };

    using ORO_CoreLib::HeartBeatGenerator;

    /**
     * @brief This DataObject refreshes the data after time S when a Get() method call is done.
     * @see DataObjectInterface
     * @see DataObject
     * @see DataRecursive
     */
    template<class _DataType>
    class DataRefreshed
        :public DataObject<_DataType>
    {
        /**
         * One element of Data.
         */
        _DataType data;
        
        /**
         * Time of data freshness.
         */
        mutable ORO_CoreLib::Seconds period;

        /**
         * Time of last measurement.
         */
        mutable HeartBeatGenerator::ticks timeStamp;
    public:
        typedef _DataType DataType;

        /**
         * Create a DataRefreshed with freshness <refresh>.
         *
         * @param refresh The maximum age of the data.
         */
        DataRefreshed(ORO_CoreLib::Seconds refresh) : period(refresh), timeStamp(0) {}
            
        void Get( DataType& pull ) const  
        { 
            if ( period < HeartBeatGenerator::timeSecondsGet(period) )
                {
                    period = HeartBeatGenerator::timeGet();
                    module->update(); 
                }
            pull = data; 
        }
            
        const DataType& Get() const
        { 
            if ( period < HeartBeatGenerator::timeSecondsGet(period) )
                {
                    period = HeartBeatGenerator::timeGet();
                    module->update(); 
                }
            return data; 
        }
            
        void Set( const DataType& push ) { data = push; }

    };


}

#endif
    
