/**
  *****************************************************************************
  * @file    Buffer_test.cpp
  * @author  Gokul Dharan
  *
  * @defgroup Buffer_test
  * @ingroup Buffer
  * @brief Buffer unit tests
  * @{
  *****************************************************************************
  */

//TODO: Fix THREADED macro issues


/********************************* Includes **********************************/
#include "UART_Handler.h"
#include "PeripheralInstances.h"
#include "MockOsInterface.h"
#include "BufferBase.h"

#include <gtest/gtest.h>
#include <gmock/gmock.h>


using ::testing::DoAll;
using ::testing::SetArgPointee;
using ::testing::Return;
using ::testing::_;

using mocks::MockOsInterface;
using namespace buffer;



/******************************** File-local *********************************/
namespace{
// Variables
// ----------------------------------------------------------------------------
MockOsInterface os;
osMutexId mutex = nullptr;

// Functions
// ----------------------------------------------------------------------------
TEST(BufferTests, CanInitializeBuffer){
    BufferBase<int> intBuffer;
    intBuffer.set_lock(mutex);
    intBuffer.set_osInterface(&os);
}

TEST(BufferTests, CanWriteToBuffer){
    BufferBase<int> intBuffer;
    intBuffer.set_lock(mutex);
    intBuffer.set_osInterface(&os);
    intBuffer.write(10);
}

TEST(BufferTests, NumReadsCheck){
    BufferBase<int> intBuffer;
    intBuffer.set_lock(mutex);
    intBuffer.set_osInterface(&os);

    ASSERT_EQ(intBuffer.num_reads() , -1);
    intBuffer.write(10);
    ASSERT_EQ(intBuffer.num_reads() , 0);
}

TEST(BufferTests, CanReadFromBuffer){
    BufferBase<int> intBuffer;
    intBuffer.set_lock(mutex);
    intBuffer.set_osInterface(&os);

    intBuffer.write(10);

    int result = intBuffer.read();
    ASSERT_EQ(result, 10);
    ASSERT_EQ(intBuffer.num_reads() , 1);

    result = intBuffer.read();
    ASSERT_EQ(result, 10);
    ASSERT_EQ(intBuffer.num_reads() , 2);
}

TEST(BufferTests, CanResetBuffer){
    BufferBase<int> intBuffer;
    intBuffer.set_lock(mutex);
    intBuffer.set_osInterface(&os);

    intBuffer.write(10);

    ASSERT_EQ(intBuffer.num_reads() , 0);
    intBuffer.reset();
    ASSERT_EQ(intBuffer.num_reads() , -1);
}

TEST(BufferTests, CanInitializeBufferMaster){
    BufferMaster bufferMaster;
    bufferMaster.setup_buffers(mutex,&os);
}

TEST(BufferTests, CanWriteToIMUBuffer){
    BufferMaster bufferMaster;
    bufferMaster.setup_buffers(mutex,&os);
    imu::IMUStruct_t IMUdata;

    bufferMaster.IMUBuffer.write(IMUdata);
}

TEST(BufferTests, CanReadFromIMUBuffer){
    BufferMaster bufferMaster;
    bufferMaster.setup_buffers(mutex,&os);
    imu::IMUStruct_t IMUdata;
    IMUdata.x_Accel = 1.0;
    IMUdata.x_Gyro = 2.0;
    IMUdata.y_Accel = 3.0;
    IMUdata.y_Gyro = 4.0;
    IMUdata.z_Accel = 5.0;
    IMUdata.z_Gyro = 6.0;

    bufferMaster.IMUBuffer.write(IMUdata);
    imu::IMUStruct_t readIMUdata = bufferMaster.IMUBuffer.read();

    ASSERT_EQ(readIMUdata.x_Accel, IMUdata.x_Accel);
    ASSERT_EQ(readIMUdata.y_Accel, IMUdata.y_Accel);
    ASSERT_EQ(readIMUdata.z_Accel, IMUdata.z_Accel);
    ASSERT_EQ(readIMUdata.x_Gyro, IMUdata.x_Gyro);
    ASSERT_EQ(readIMUdata.y_Gyro, IMUdata.y_Gyro);
    ASSERT_EQ(readIMUdata.z_Gyro, IMUdata.z_Gyro);
}

TEST(BufferTests, CanWriteToMotorBuffer){
    //TODO: Use periph::NUM_MOTORS without defining THREADED
    BufferMaster bufferMaster;
    bufferMaster.setup_buffers(mutex,&os);
    MotorData_t motorData[periph::NUM_MOTORS];

    for(int i = 0; i < periph::NUM_MOTORS; ++i)
    {
        bufferMaster.MotorBufferArray[i].write(motorData[i]);
    }
}

TEST(BufferTests, CanReadMotorDataBuffer){
    BufferMaster bufferMaster;
    bufferMaster.setup_buffers(mutex,&os);
    MotorData_t motorData[periph::NUM_MOTORS];
    MotorData_t readMotorData[periph::NUM_MOTORS];

    for(int i = 0; i < periph::NUM_MOTORS; ++i)
    {
        motorData[i].id = i;
        bufferMaster.MotorBufferArray[i].write(motorData[i]);
        readMotorData[i] = bufferMaster.MotorBufferArray[i].read();
        ASSERT_EQ(readMotorData[i].id, i);
    }
}

TEST(BufferTests, CanConfirmAllDataReady){
    BufferMaster bufferMaster;
    bufferMaster.setup_buffers(mutex,&os);
    MotorData_t motorData[periph::NUM_MOTORS];
    imu::IMUStruct_t IMUdata;

    ASSERT_FALSE(bufferMaster.all_data_ready());
    for(int i = 0; i < periph::NUM_MOTORS; ++i)
    {
        bufferMaster.MotorBufferArray[i].write(motorData[i]);
        ASSERT_FALSE(bufferMaster.all_data_ready());
    }

    bufferMaster.IMUBuffer.write(IMUdata);
    ASSERT_TRUE(bufferMaster.all_data_ready());

    imu::IMUStruct_t readIMUdata = bufferMaster.IMUBuffer.read();
    ASSERT_FALSE(bufferMaster.all_data_ready());

    bufferMaster.IMUBuffer.write(IMUdata);
    ASSERT_TRUE(bufferMaster.all_data_ready());
    MotorData_t readMotorData = bufferMaster.MotorBufferArray[0].read();
    ASSERT_FALSE(bufferMaster.all_data_ready());
}

} // end anonymous namespace

/**
 * @}
 */
/* end - Buffer_test */
