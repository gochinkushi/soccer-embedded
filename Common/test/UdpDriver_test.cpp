/**
  *****************************************************************************
  * @file    UdpDriver_test.cpp
  * @author  Robert Fairley
  *
  * @defgroup udp_driver_test
  * @ingroup  udp_driver
  * @brief    Unit tests for the UdpDriver class.
  * @{
  *****************************************************************************
  */

/* TODO: investigate threaded tests. */



/********************************* Includes **********************************/
#include <MockOsInterface.h>
#include <MockUdpInterface.h>
#include <UdpDriver.h>

#include <gtest/gtest.h>
#include <gmock/gmock.h>




using udp_driver::UdpDriver;
using mocks::MockUdpInterface;
using mocks::MockOsInterface;

using ::testing::Return;
using ::testing::_;
using ::testing::AnyNumber;




/* NOTE: needs to be defined outside of anonymous
namespace so gtest can use it. */
bool operator==(const ip_addr_t& lhs, const ip_addr_t& rhs) {
    return lhs.addr == rhs.addr;
}




/******************************** File-local *********************************/
namespace {
// Constants
// ----------------------------------------------------------------------------
const ip_addr_t ZERO_IP_ADDR_T = {0x0};

}




TEST(UdpDriverShould, DefaultInitializeMembersToZero) {
    UdpDriver udpDriverUnderTest;

    EXPECT_EQ(udpDriverUnderTest.getIpaddr(), ZERO_IP_ADDR_T);
    EXPECT_EQ(udpDriverUnderTest.getIpaddrPc(), ZERO_IP_ADDR_T);
    EXPECT_EQ(udpDriverUnderTest.getPort(), (u16_t) 0);
    EXPECT_EQ(udpDriverUnderTest.getPortPc(), (u16_t) 0);
    EXPECT_EQ(udpDriverUnderTest.getUdpInterface(), nullptr);
    EXPECT_EQ(udpDriverUnderTest.getOsInterface(), nullptr);
}

TEST(UdpDriverShould, InitializeMembersWithParameterizedConstructor) {
    MockUdpInterface udp_if;
    MockOsInterface os_if;
    const ip_addr_t TEST_IP_ADDR = {0xC0A80008};
    const ip_addr_t TEST_IP_ADDR_PC = {0xC0A80002};

    UdpDriver udpDriverUnderTest(TEST_IP_ADDR, TEST_IP_ADDR_PC, (u16_t) 7,
            (u16_t) 6340, &udp_if, &os_if);

    EXPECT_EQ(TEST_IP_ADDR, udpDriverUnderTest.getIpaddr());
    EXPECT_EQ(TEST_IP_ADDR_PC, udpDriverUnderTest.getIpaddrPc());
    EXPECT_EQ((u16_t) 7, udpDriverUnderTest.getPort());
    EXPECT_EQ((u16_t) 6340, udpDriverUnderTest.getPortPc());
    EXPECT_EQ(&udp_if, udpDriverUnderTest.getUdpInterface());
    EXPECT_EQ(&os_if, udpDriverUnderTest.getOsInterface());
}

TEST(UdpDriverShould, SucceedInitialize) {
    MockUdpInterface udp_if;
    MockOsInterface os_if;
    const ip_addr_t TEST_IP_ADDR = {0xC0A80008};
    const ip_addr_t TEST_IP_ADDR_PC = {0xC0A80002};

    EXPECT_CALL(os_if, OS_osSemaphoreCreate(_, _)).Times(1).WillOnce(Return((osSemaphoreId) 1));
    EXPECT_CALL(os_if, OS_osMutexCreate(_)).Times(1).WillOnce(Return((osMutexId) 1));

    UdpDriver udpDriverUnderTest(TEST_IP_ADDR, TEST_IP_ADDR_PC, (u16_t) 7,
            (u16_t) 6340, &udp_if, &os_if);

    ASSERT_TRUE(udpDriverUnderTest.initialize());
}

TEST(UdpDriverShould, FailInitializeWhenUdpInterfaceNull) {
    MockOsInterface os_if;
    const ip_addr_t TEST_IP_ADDR = {0xC0A80008};
    const ip_addr_t TEST_IP_ADDR_PC = {0xC0A80002};

    UdpDriver udpDriverUnderTest(TEST_IP_ADDR, TEST_IP_ADDR_PC, (u16_t) 7,
            (u16_t) 6340, nullptr, &os_if);

    ASSERT_FALSE(udpDriverUnderTest.initialize());
}

TEST(UdpDriverShould, FailInitializeWhenOsInterfaceNull) {
    MockUdpInterface udp_if;
    const ip_addr_t TEST_IP_ADDR = {0xC0A80008};
    const ip_addr_t TEST_IP_ADDR_PC = {0xC0A80002};

    UdpDriver udpDriverUnderTest(TEST_IP_ADDR, TEST_IP_ADDR_PC, (u16_t) 7,
            (u16_t) 6340, &udp_if, nullptr);

    ASSERT_FALSE(udpDriverUnderTest.initialize());
}

TEST(UdpDriverShould, FailInitializeWhenMutexCreateUnsuccessful) {
    MockUdpInterface udp_if;
    MockOsInterface os_if;
    const ip_addr_t TEST_IP_ADDR = {0xC0A80008};
    const ip_addr_t TEST_IP_ADDR_PC = {0xC0A80002};

    EXPECT_CALL(os_if, OS_osSemaphoreCreate(_, _)).Times(AnyNumber()).WillOnce(Return((osSemaphoreId) 1));
    EXPECT_CALL(os_if, OS_osMutexCreate(_)).Times(1).WillOnce(Return((osMutexId) NULL));

    UdpDriver udpDriverUnderTest(TEST_IP_ADDR, TEST_IP_ADDR_PC, (u16_t) 7,
            (u16_t) 6340, &udp_if, &os_if);

    ASSERT_FALSE(udpDriverUnderTest.initialize());
}

TEST(UdpDriverShould, FailInitializeWhenSemaphoreCreateUnsuccessful) {
    MockUdpInterface udp_if;
    MockOsInterface os_if;
    const ip_addr_t TEST_IP_ADDR = {0xC0A80008};
    const ip_addr_t TEST_IP_ADDR_PC = {0xC0A80002};

    EXPECT_CALL(os_if, OS_osSemaphoreCreate(_, _)).Times(1).WillOnce(Return((osSemaphoreId) NULL));
    EXPECT_CALL(os_if, OS_osMutexCreate(_)).Times(AnyNumber()).WillOnce(Return((osMutexId) 1));

    UdpDriver udpDriverUnderTest(TEST_IP_ADDR, TEST_IP_ADDR_PC, (u16_t) 7,
            (u16_t) 6340, &udp_if, &os_if);

    ASSERT_FALSE(udpDriverUnderTest.initialize());
}

TEST(UdpDriverShould, SucceedSetupReceive) {
    MockUdpInterface udp_if;
    MockOsInterface os_if;
    struct udp_pcb udpPcb;

    EXPECT_CALL(udp_if, udpRecv(_, _, _)).Times(1);
    EXPECT_CALL(udp_if, udpBind(_, _, _)).Times(1).WillOnce(Return(ERR_OK));
    EXPECT_CALL(udp_if, udpNew()).Times(1).WillOnce(Return(&udpPcb));

    UdpDriver udpDriverUnderTest(ZERO_IP_ADDR_T, ZERO_IP_ADDR_T, (u16_t) 0, (u16_t) 0,
            &udp_if, &os_if);
    ASSERT_TRUE(udpDriverUnderTest.setupReceive(nullptr));
    EXPECT_EQ(udpDriverUnderTest.getPcb(), &udpPcb);
}

TEST(UdpDriverShould, FailSetupReceiveOnUdpNew) {
    MockUdpInterface udp_if;
    MockOsInterface os_if;
    EXPECT_CALL(udp_if, udpNew()).Times(1).WillOnce(Return(nullptr));

    UdpDriver udpDriverUnderTest(ZERO_IP_ADDR_T, ZERO_IP_ADDR_T, (u16_t) 0, (u16_t) 0,
            &udp_if, &os_if);
    ASSERT_FALSE(udpDriverUnderTest.setupReceive(nullptr));
    EXPECT_EQ(udpDriverUnderTest.getPcb(), nullptr);
}

TEST(UdpDriverShould, FailSetupReceiveOnUdpBind) {
    MockUdpInterface udp_if;
    MockOsInterface os_if;
    struct udp_pcb udpPcb;

    EXPECT_CALL(udp_if, udpRemove(_)).Times(1);
    EXPECT_CALL(udp_if, udpBind(_, _, _)).Times(1).WillOnce(Return(ERR_USE));
    EXPECT_CALL(udp_if, udpNew()).Times(1).WillOnce(Return(&udpPcb));

    UdpDriver udpDriverUnderTest(ZERO_IP_ADDR_T, ZERO_IP_ADDR_T, (u16_t) 0, (u16_t) 0,
            &udp_if, &os_if);
    ASSERT_FALSE(udpDriverUnderTest.setupReceive(nullptr));

    /* Check the PCB has been removed. */
    EXPECT_EQ(udpDriverUnderTest.getPcb(), nullptr);
}

TEST(UdpDriverShould, SucceedReceive) {
    MockUdpInterface udp_if;
    MockOsInterface os_if;
    struct pbuf rxPbuf;

    uint8_t rxBuff[10] = {};

    EXPECT_CALL(os_if, OS_osMutexWait(_, _)).WillRepeatedly(Return(osOK));

    EXPECT_CALL(udp_if, pbufFree(_)).Times(1);
    EXPECT_CALL(udp_if, pbufCopyPartial(_, _, _, _)).Times(1).WillOnce(Return((u16_t) 1));
    EXPECT_CALL(os_if, OS_osSemaphoreWait(_, _)).Times(1).WillOnce(Return(osOK));

    UdpDriver udpDriverUnderTest(ZERO_IP_ADDR_T, ZERO_IP_ADDR_T, (u16_t) 0, (u16_t) 0,
            &udp_if, &os_if);

    udpDriverUnderTest.setRecvPbuf(&rxPbuf);

    ASSERT_TRUE(udpDriverUnderTest.receive(rxBuff, sizeof(rxBuff)));
}

TEST(UdpDriverShould, FailReceiveRxArrayNull) {
    MockUdpInterface udp_if;
    MockOsInterface os_if;
    struct pbuf rxPbuf;

    EXPECT_CALL(os_if, OS_osMutexWait(_, _)).WillRepeatedly(Return(osOK));

    EXPECT_CALL(os_if, OS_osSemaphoreWait(_, _)).Times(1).WillOnce(Return(osOK));

    UdpDriver udpDriverUnderTest(ZERO_IP_ADDR_T, ZERO_IP_ADDR_T, (u16_t) 0, (u16_t) 0,
            &udp_if, &os_if);

    udpDriverUnderTest.setRecvPbuf(&rxPbuf);

    ASSERT_FALSE(udpDriverUnderTest.receive(NULL, 10));
}

TEST(UdpDriverShould, FailReceiveRxPbufNull) {
    MockUdpInterface udp_if;
    MockOsInterface os_if;
    uint8_t rxBuff[10] = {};

    EXPECT_CALL(os_if, OS_osMutexWait(_, _)).WillRepeatedly(Return(osOK));

    EXPECT_CALL(os_if, OS_osSemaphoreWait(_, _)).Times(1).WillOnce(Return(osOK));

    UdpDriver udpDriverUnderTest(ZERO_IP_ADDR_T, ZERO_IP_ADDR_T, (u16_t) 0, (u16_t) 0,
            &udp_if, &os_if);

    udpDriverUnderTest.setRecvPbuf(NULL);

    ASSERT_FALSE(udpDriverUnderTest.receive(rxBuff, sizeof(rxBuff)));
}

TEST(UdpDriverShould, FailReceiveZeroBytesCopied) {
    MockUdpInterface udp_if;
    MockOsInterface os_if;
    uint8_t rxBuff[10] = {};
    struct pbuf rxPbuf;

    EXPECT_CALL(os_if, OS_osMutexWait(_, _)).WillRepeatedly(Return(osOK));

    EXPECT_CALL(udp_if, pbufCopyPartial(_, _, _, _)).Times(1).WillOnce(Return((u16_t) 0));
    EXPECT_CALL(os_if, OS_osSemaphoreWait(_, _)).Times(1).WillOnce(Return(osOK));

    UdpDriver udpDriverUnderTest(ZERO_IP_ADDR_T, ZERO_IP_ADDR_T, (u16_t) 0, (u16_t) 0,
            &udp_if, &os_if);

    udpDriverUnderTest.setRecvPbuf(&rxPbuf);

    ASSERT_FALSE(udpDriverUnderTest.receive(rxBuff, sizeof(rxBuff)));
}

TEST(UdpDriverShould, SucceedTransmit) {
    MockUdpInterface udp_if;
    MockOsInterface os_if;
    uint8_t txBuff[10] = {};
    struct pbuf txPbuf;
    const ip_addr_t TEST_IP_ADDR = {0xC0A80008};
    const ip_addr_t TEST_IP_ADDR_PC = {0xC0A80002};

    EXPECT_CALL(udp_if, pbufFree(&txPbuf)).Times(1).WillOnce(Return((u8_t) 1));
    EXPECT_CALL(udp_if, udpDisconnect(_)).Times(1);
    EXPECT_CALL(udp_if, udpSend(_, _)).Times(1).WillOnce(Return(ERR_OK));
    EXPECT_CALL(udp_if, udpConnect(_, _, _)).Times(1).WillOnce(Return(ERR_OK));
    EXPECT_CALL(udp_if, pbufTake(_, _, _)).Times(1).WillOnce(Return(ERR_OK));
    EXPECT_CALL(udp_if, pbufAlloc(PBUF_TRANSPORT, _, PBUF_RAM)).Times(1).WillOnce(Return(&txPbuf));

    UdpDriver udpDriverUnderTest(TEST_IP_ADDR, TEST_IP_ADDR_PC, (u16_t) 7,
            (u16_t) 6340, &udp_if, &os_if);

    ASSERT_TRUE(udpDriverUnderTest.transmit(txBuff, sizeof(txBuff)));
}

TEST(UdpDriverShould, FailTransmitWhenTxBuffNull) {
    MockUdpInterface udp_if;
    MockOsInterface os_if;
    struct pbuf txPbuf;
    const ip_addr_t TEST_IP_ADDR = {0xC0A80008};
    const ip_addr_t TEST_IP_ADDR_PC = {0xC0A80002};

    EXPECT_CALL(udp_if, pbufTake(_, _, _)).Times(0);
    EXPECT_CALL(udp_if, pbufAlloc(PBUF_TRANSPORT, _, PBUF_RAM)).Times(1).WillOnce(Return(&txPbuf));

    UdpDriver udpDriverUnderTest(TEST_IP_ADDR, TEST_IP_ADDR_PC, (u16_t) 7,
            (u16_t) 6340, &udp_if, &os_if);

    ASSERT_FALSE(udpDriverUnderTest.transmit(nullptr, 10));
}

TEST(UdpDriverShould, FailTransmitWhenPbufAllocUnsuccessful) {
    MockUdpInterface udp_if;
    MockOsInterface os_if;
    uint8_t txBuff[10] = {};
    const ip_addr_t TEST_IP_ADDR = {0xC0A80008};
    const ip_addr_t TEST_IP_ADDR_PC = {0xC0A80002};

    EXPECT_CALL(udp_if, pbufFree(_)).Times(0);
    EXPECT_CALL(udp_if, pbufAlloc(PBUF_TRANSPORT, _, PBUF_RAM)).Times(1).WillOnce(Return((struct pbuf *) NULL));

    UdpDriver udpDriverUnderTest(TEST_IP_ADDR, TEST_IP_ADDR_PC, (u16_t) 7,
            (u16_t) 6340, &udp_if, &os_if);

    ASSERT_FALSE(udpDriverUnderTest.transmit(txBuff, sizeof(txBuff)));
}

TEST(UdpDriverShould, FailTransmitWhenPbufTakeUnsuccessful) {
    MockUdpInterface udp_if;
    MockOsInterface os_if;
    uint8_t txBuff[10] = {};
    struct pbuf txPbuf;
    const ip_addr_t TEST_IP_ADDR = {0xC0A80008};
    const ip_addr_t TEST_IP_ADDR_PC = {0xC0A80002};

    EXPECT_CALL(udp_if, pbufTake(_, _, _)).Times(1).WillOnce(Return(ERR_MEM));
    EXPECT_CALL(udp_if, pbufAlloc(PBUF_TRANSPORT, _, PBUF_RAM)).Times(1).WillOnce(Return(&txPbuf));

    UdpDriver udpDriverUnderTest(TEST_IP_ADDR, TEST_IP_ADDR_PC, (u16_t) 7,
            (u16_t) 6340, &udp_if, &os_if);

    ASSERT_FALSE(udpDriverUnderTest.transmit(txBuff, sizeof(txBuff)));
}

TEST(UdpDriverShould, FailTransmitWhenUdpConnectUnsuccessful) {
    MockUdpInterface udp_if;
    MockOsInterface os_if;
    uint8_t txBuff[10] = {};
    struct pbuf txPbuf;
    const ip_addr_t TEST_IP_ADDR = {0xC0A80008};
    const ip_addr_t TEST_IP_ADDR_PC = {0xC0A80002};

    EXPECT_CALL(udp_if, udpConnect(_, _, _)).Times(1).WillOnce(Return(ERR_VAL));
    EXPECT_CALL(udp_if, pbufTake(_, _, _)).Times(1).WillOnce(Return(ERR_OK));
    EXPECT_CALL(udp_if, pbufAlloc(PBUF_TRANSPORT, _, PBUF_RAM)).Times(1).WillOnce(Return(&txPbuf));

    UdpDriver udpDriverUnderTest(TEST_IP_ADDR, TEST_IP_ADDR_PC, (u16_t) 7,
            (u16_t) 6340, &udp_if, &os_if);

    ASSERT_FALSE(udpDriverUnderTest.transmit(txBuff, sizeof(txBuff)));
}

TEST(UdpDriverShould, FailTransmitWhenUdpSendUnsuccessful) {
    MockUdpInterface udp_if;
    MockOsInterface os_if;
    uint8_t txBuff[10] = {};
    struct pbuf txPbuf;
    const ip_addr_t TEST_IP_ADDR = {0xC0A80008};
    const ip_addr_t TEST_IP_ADDR_PC = {0xC0A80002};

    EXPECT_CALL(udp_if, udpDisconnect(_)).Times(1);
    EXPECT_CALL(udp_if, udpSend(_, _)).Times(1).WillOnce(Return(ERR_VAL));
    EXPECT_CALL(udp_if, udpConnect(_, _, _)).Times(1).WillOnce(Return(ERR_OK));
    EXPECT_CALL(udp_if, pbufTake(_, _, _)).Times(1).WillOnce(Return(ERR_OK));
    EXPECT_CALL(udp_if, pbufAlloc(PBUF_TRANSPORT, _, PBUF_RAM)).Times(1).WillOnce(Return(&txPbuf));

    UdpDriver udpDriverUnderTest(TEST_IP_ADDR, TEST_IP_ADDR_PC, (u16_t) 7,
            (u16_t) 6340, &udp_if, &os_if);

    ASSERT_FALSE(udpDriverUnderTest.transmit(txBuff, sizeof(txBuff)));
}
