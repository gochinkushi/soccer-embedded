/**
  *****************************************************************************
  * @file    BufferBase.h
  * @author  Gokul Dharan
  * @brief   Defines an abstract buffer class to be implemented and extended as needed
  *
  * @defgroup Buffer
  * @{
  *****************************************************************************
  */


#ifndef BUFFER_BASE_H
#define BUFFER_BASE_H

/********************************** Includes **********************************/
#include "PeripheralInstances.h"



namespace buffer {


/*********************************** Buffer ***********************************/
/**
 * @class BufferBase Generic templated thread-safe buffer class
 */
template <class T>
class BufferBase
{
public:
    BufferBase() {}
    ~BufferBase() {}
    void set_osInterface(os::OsInterface *osInterface)
    {
        m_osInterfacePtr = osInterface;
    }
    void set_lock(osMutexId lock)
    {
        m_lock = lock;
    }
    void write(const T &item)
    {
        m_osInterfacePtr->OS_xSemaphoreTake(m_lock, osWaitForever);
        m_databuf = item;
        m_read = 0;
        m_osInterfacePtr->OS_xSemaphoreGive(m_lock);
    }
    T read()
    {
        m_osInterfacePtr->OS_xSemaphoreTake(m_lock, osWaitForever);
        m_read++;
        m_osInterfacePtr->OS_xSemaphoreGive(m_lock);
        return m_databuf;
    }
    void reset()
    {
        m_osInterfacePtr->OS_xSemaphoreTake(m_lock, osWaitForever);
        m_read = -1;
        m_osInterfacePtr->OS_xSemaphoreGive(m_lock);
    }
    int8_t num_reads()
    {
        return m_read;
    }

    // Defining methods here in the declaration for ease of use as a templated class
private:
    T m_databuf;
    //int indicates whether data has been read, -1 if data not written yet
    int8_t m_read = -1;
    osMutexId m_lock = nullptr;
    os::OsInterface* m_osInterfacePtr = nullptr;
};


/**
 * @class BufferMaster Easily extendible thread-safe master class that holds
 *        all relevant buffers
 */

class BufferMaster
{
public:
    BufferMaster() {}
    ~BufferMaster() {}
    void setup_buffers(osMutexId lock, os::OsInterface *osInterface)
    {
        IMUBuffer.set_lock(lock);
        IMUBuffer.set_osInterface(osInterface);
        for(int i = 0; i < periph::NUM_MOTORS; ++i)
        {
            MotorBufferArray[i].set_lock(lock);
            MotorBufferArray[i].set_osInterface(osInterface);
        }
        m_lock = lock;
        m_osInterfacePtr = osInterface;
    }
    bool all_data_ready()
    {
        m_osInterfacePtr->OS_xSemaphoreTake(m_lock, osWaitForever);
        bool ready =  (IMUBuffer.num_reads() == 0);

        if(ready)
        {
            for(int i = 0; i < periph::NUM_MOTORS; ++i)
            {
                ready = (ready && MotorBufferArray[i].num_reads() == 0);
            }
        }
        m_osInterfacePtr->OS_xSemaphoreGive(m_lock);
        return ready;
    }
    BufferBase<imu::IMUStruct_t> IMUBuffer;
    BufferBase<MotorData_t> MotorBufferArray[periph::NUM_MOTORS];
    // Add buffer items here as necessary
private:
    osMutexId m_lock = nullptr;
    os::OsInterface* m_osInterfacePtr = nullptr;
};

} // end namespace buffer

/**
 * @}
 */
/* end - BufferBase */

#endif /* BUFFER_BASE_H */
