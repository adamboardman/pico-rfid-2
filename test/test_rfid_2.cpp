#include <catch2/catch_test_macros.hpp>
#include <cstdarg>
#include <cstring>
#include <vector>

#include "rfid_2.h"

i2c_inst_t *i2c0 = nullptr;
RFID_2 rfid_2(i2c0, 0x28); //NOLINT

TEST_CASE("Init - Antenna Off", "[rfid_2]") {
    reset_for_test(i2c0);
    std::vector<uint8_t> data = {0b00000000, 0b00000000, 0b00000000};
    set_read_data(data);
    rfid_2.PCD_Init();
    // REQUIRE(ret == PICO_ERROR_NONE);
    REQUIRE(lastAddress == 0x28);
    REQUIRE(mock_data_read == 2);
    REQUIRE(last_length_written == 2);
    REQUIRE(mock_write_data.size() == 18);
    int read = 0;
    REQUIRE(mock_write_data.at(read++) == rfid_2.CommandReg);
    REQUIRE(mock_write_data.at(read++) == rfid_2.PCD_SoftReset);
    REQUIRE(mock_write_data.at(read++) == rfid_2.CommandReg);
    REQUIRE(mock_write_data.at(read++) == rfid_2.TModeReg);
    REQUIRE(mock_write_data.at(read++) == 0x80);
    REQUIRE(mock_write_data.at(read++) == rfid_2.TPrescalerReg);
    REQUIRE(mock_write_data.at(read++) == 0xA9);
    REQUIRE(mock_write_data.at(read++) == rfid_2.TReloadRegH);
    REQUIRE(mock_write_data.at(read++) == 0x03);
    REQUIRE(mock_write_data.at(read++) == rfid_2.TReloadRegL);
    REQUIRE(mock_write_data.at(read++) == 0xE8);
    REQUIRE(mock_write_data.at(read++) == rfid_2.TxASKReg);
    REQUIRE(mock_write_data.at(read++) == 0x40);
    REQUIRE(mock_write_data.at(read++) == rfid_2.ModeReg);
    REQUIRE(mock_write_data.at(read++) == 0x3D);
    REQUIRE(mock_write_data.at(read++) == rfid_2.TxControlReg);
    REQUIRE(mock_write_data.at(read++) == rfid_2.TxControlReg);
    REQUIRE(mock_write_data.at(read++) == 0x03);
	REQUIRE(mock_write_data.size() == read);
}

TEST_CASE("Init - Antenna On already", "[rfid_2]") {
    reset_for_test(i2c0);
    std::vector<uint8_t> data = {
        0b00000000, // check for power down bit after soft reset
        0b00000011, // antenna state within control register
        0b00000000
    };
    set_read_data(data);
    rfid_2.PCD_Init();

    REQUIRE(lastAddress == 0x28);
    REQUIRE(mock_data_read == 2);
    REQUIRE(last_length_written == 1);
    REQUIRE(mock_write_data.size() == 16);
    int read = 0;
    REQUIRE(mock_write_data.at(read++) == rfid_2.CommandReg);
    REQUIRE(mock_write_data.at(read++) == rfid_2.PCD_SoftReset);
    REQUIRE(mock_write_data.at(read++) == rfid_2.CommandReg);
    REQUIRE(mock_write_data.at(read++) == rfid_2.TModeReg);
    REQUIRE(mock_write_data.at(read++) == 0x80);
    REQUIRE(mock_write_data.at(read++) == rfid_2.TPrescalerReg);
    REQUIRE(mock_write_data.at(read++) == 0xA9);
    REQUIRE(mock_write_data.at(read++) == rfid_2.TReloadRegH);
    REQUIRE(mock_write_data.at(read++) == 0x03);
    REQUIRE(mock_write_data.at(read++) == rfid_2.TReloadRegL);
    REQUIRE(mock_write_data.at(read++) == 0xE8);
    REQUIRE(mock_write_data.at(read++) == rfid_2.TxASKReg);
    REQUIRE(mock_write_data.at(read++) == 0x40);
    REQUIRE(mock_write_data.at(read++) == rfid_2.ModeReg);
    REQUIRE(mock_write_data.at(read++) == 0x3D);
    REQUIRE(mock_write_data.at(read++) == rfid_2.TxControlReg);
	REQUIRE(mock_write_data.size() == read);
}

TEST_CASE("Init - PowerDown bit still set", "[rfid_2]") {
    reset_for_test(i2c0);
    std::vector<uint8_t> data = {
        0b00010000, // PowerDown bit still set
        0b00000000, // cleared
        0b00000011, // antenna state within control register
        0b00000000
    };
    set_read_data(data);
    rfid_2.PCD_Init();

    REQUIRE(lastAddress == 0x28);
    REQUIRE(mock_data_read == 3);
    REQUIRE(last_length_written == 1);
    REQUIRE(mock_write_data.size() == 17);
    int read = 0;
    REQUIRE((int)mock_write_data.at(read++) == rfid_2.CommandReg);
    REQUIRE((int)mock_write_data.at(read++) == rfid_2.PCD_SoftReset);
    REQUIRE((int)mock_write_data.at(read++) == rfid_2.CommandReg);
    REQUIRE((int)mock_write_data.at(read++) == rfid_2.CommandReg);
    REQUIRE((int)mock_write_data.at(read++) == rfid_2.TModeReg);
    REQUIRE((int)mock_write_data.at(read++) == 0x80);
    REQUIRE((int)mock_write_data.at(read++) == rfid_2.TPrescalerReg);
    REQUIRE((int)mock_write_data.at(read++) == 0xA9);
    REQUIRE((int)mock_write_data.at(read++) == rfid_2.TReloadRegH);
    REQUIRE((int)mock_write_data.at(read++) == 0x03);
    REQUIRE((int)mock_write_data.at(read++) == rfid_2.TReloadRegL);
    REQUIRE((int)mock_write_data.at(read++) == 0xE8);
    REQUIRE((int)mock_write_data.at(read++) == rfid_2.TxASKReg);
    REQUIRE((int)mock_write_data.at(read++) == 0x40);
    REQUIRE((int)mock_write_data.at(read++) == rfid_2.ModeReg);
    REQUIRE((int)mock_write_data.at(read++) == 0x3D);
    REQUIRE((int)mock_write_data.at(read++) == rfid_2.TxControlReg);
	REQUIRE(mock_write_data.size() == read);
}

TEST_CASE("Init - Is a card present - false", "[rfid_2]") {
    reset_for_test(i2c0);
    std::vector<uint8_t> dataForNewCard = {
        0b00000000, // ClearRegisterBitMask - CollReg
        0b00000000, // SetRegisterBitMask - FIFOLevelReg
        0b00000000, // SetRegisterBitMask - BitFramingReg
        0b00110000, // ComIrqReg - RxIRq and IdleIRq(0x30)
        0b00010011, // ErrorReg - ErrorReg[7..0] bits are:
        //WrErr TempErr reserved BufferOverflow CollErr CRCErr ParityErr ProtocolErr
    };
    set_read_data(dataForNewCard);

    bool present = rfid_2.PICC_IsNewCardPresent();
    REQUIRE(present == false);
    REQUIRE(lastAddress == 0x28);
    REQUIRE(lastAddress == 0x28);
    REQUIRE(mock_data_read == 5);
    REQUIRE(last_length_written == 1);
    REQUIRE(mock_write_data.size() == 21);
    int read = 0;
    REQUIRE((int)mock_write_data.at(read++) == rfid_2.CollReg);
    REQUIRE((int)mock_write_data.at(read++) == rfid_2.CollReg);
    REQUIRE((int)mock_write_data.at(read++) == 0); //PCD_ClearRegisterBitMask
    REQUIRE((int)mock_write_data.at(read++) == rfid_2.CommandReg);
	REQUIRE((int)mock_write_data.at(read++) == rfid_2.PCD_Idle);
    REQUIRE((int)mock_write_data.at(read++) == rfid_2.ComIrqReg);
    REQUIRE((int)mock_write_data.at(read++) == 0x7F);
    REQUIRE((int)mock_write_data.at(read++) == rfid_2.FIFOLevelReg);
    REQUIRE((int)mock_write_data.at(read++) == rfid_2.FIFOLevelReg);
    REQUIRE((int)mock_write_data.at(read++) == 0x80);
	REQUIRE((int)mock_write_data.at(read++) == RFID_2::FIFODataReg);
	REQUIRE((int)mock_write_data.at(read++) == rfid_2.PICC_CMD_REQA);
	REQUIRE((int)mock_write_data.at(read++) == rfid_2.BitFramingReg);
	REQUIRE((int)mock_write_data.at(read++) == rfid_2.Status1Reg);
    REQUIRE((int)mock_write_data.at(read++) == rfid_2.CommandReg);
    REQUIRE((int)mock_write_data.at(read++) == rfid_2.ControlReg);
    REQUIRE((int)mock_write_data.at(read++) == rfid_2.BitFramingReg);
    REQUIRE((int)mock_write_data.at(read++) == rfid_2.BitFramingReg);
    REQUIRE((int)mock_write_data.at(read++) == 0x80);
    REQUIRE((int)mock_write_data.at(read++) == rfid_2.ComIrqReg);
    REQUIRE((int)mock_write_data.at(read++) == rfid_2.ErrorReg);
	REQUIRE(mock_write_data.size() == read);

}

TEST_CASE("Init - Is a card present - true", "[rfid_2]") {
	reset_for_test(i2c0);
	std::vector<uint8_t> readData;
	std::vector<uint8_t> expectedWriteData;
	expectedWriteData.push_back(RFID_2::CollReg); //ReadRegister
	readData.push_back(0b00000000); // ClearRegisterBitMask - CollReg
    expectedWriteData.push_back(RFID_2::CollReg); //WriteRegister
	expectedWriteData.push_back(0);

	//PCD_CommunicateWithPICC
	expectedWriteData.push_back(RFID_2::CommandReg);
	expectedWriteData.push_back(RFID_2::PCD_Idle);
	expectedWriteData.push_back(RFID_2::ComIrqReg);
	expectedWriteData.push_back(0x7F);
	expectedWriteData.push_back(RFID_2::FIFOLevelReg);
	readData.push_back(0b00000000); // SetRegisterBitMask - FIFOLevelReg
	expectedWriteData.push_back(RFID_2::FIFOLevelReg);
	expectedWriteData.push_back(0x80);
	expectedWriteData.push_back(RFID_2::FIFODataReg);
	expectedWriteData.push_back(RFID_2::PICC_CMD_REQA);
	expectedWriteData.push_back(RFID_2::BitFramingReg);
	expectedWriteData.push_back(RFID_2::Status1Reg);
	expectedWriteData.push_back(RFID_2::CommandReg);
	expectedWriteData.push_back(RFID_2::ControlReg);
	expectedWriteData.push_back(RFID_2::BitFramingReg);
    readData.push_back(0b00000000); // SetRegisterBitMask - BitFramingReg
	expectedWriteData.push_back(RFID_2::BitFramingReg);
	expectedWriteData.push_back(0x80);
	expectedWriteData.push_back(RFID_2::ComIrqReg);
    readData.push_back(0b00110000); // ComIrqReg - RxIRq and IdleIRq(0x30)
	expectedWriteData.push_back(RFID_2::ErrorReg);
	readData.push_back(0b00000000); // ErrorReg - no errors

	expectedWriteData.push_back(RFID_2::FIFOLevelReg);
	readData.push_back(2); // FIFOLevelReg - number of bytes in the FIFO
	expectedWriteData.push_back(RFID_2::FIFODataReg);
	readData.push_back(0b00000000); // first byte
    readData.push_back(0b00000000); // second byte
	expectedWriteData.push_back(RFID_2::ControlReg);
    readData.push_back(0b00000000); // ControlReg - to check valid bits - 000b, the whole byte is valid
	set_read_data(readData);
	set_expected_write_data(expectedWriteData);

    bool present = rfid_2.PICC_IsNewCardPresent();
    REQUIRE(present == true);
    REQUIRE(lastAddress == 0x28);
    REQUIRE(lastAddress == 0x28);
    REQUIRE(mock_data_read == 9);
    REQUIRE(last_length_written == 1);
    REQUIRE(mock_write_data.size() == 24);
    int read = 0;
	REQUIRE((int)mock_write_data.at(read++) == rfid_2.CollReg);
	REQUIRE((int)mock_write_data.at(read++) == rfid_2.CollReg);
	REQUIRE((int)mock_write_data.at(read++) == 0); //PCD_ClearRegisterBitMask
	REQUIRE((int)mock_write_data.at(read++) == rfid_2.CommandReg);
	REQUIRE((int)mock_write_data.at(read++) == rfid_2.PCD_Idle);
	REQUIRE((int)mock_write_data.at(read++) == rfid_2.ComIrqReg);
	REQUIRE((int)mock_write_data.at(read++) == 0x7F);
	REQUIRE((int)mock_write_data.at(read++) == rfid_2.FIFOLevelReg);
	REQUIRE((int)mock_write_data.at(read++) == rfid_2.FIFOLevelReg);
	REQUIRE((int)mock_write_data.at(read++) == 0x80);
	REQUIRE((int)mock_write_data.at(read++) == RFID_2::FIFODataReg);
	REQUIRE((int)mock_write_data.at(read++) == rfid_2.PICC_CMD_REQA);
	REQUIRE((int)mock_write_data.at(read++) == rfid_2.BitFramingReg);
	REQUIRE((int)mock_write_data.at(read++) == rfid_2.Status1Reg);
	REQUIRE((int)mock_write_data.at(read++) == rfid_2.CommandReg);
	REQUIRE((int)mock_write_data.at(read++) == rfid_2.ControlReg);
	REQUIRE((int)mock_write_data.at(read++) == rfid_2.BitFramingReg);
	REQUIRE((int)mock_write_data.at(read++) == rfid_2.BitFramingReg);
	REQUIRE((int)mock_write_data.at(read++) == 0x80);
	REQUIRE((int)mock_write_data.at(read++) == rfid_2.ComIrqReg);
	REQUIRE((int)mock_write_data.at(read++) == rfid_2.ErrorReg);
	REQUIRE((int)mock_write_data.at(read++) == rfid_2.FIFOLevelReg);
	REQUIRE((int)mock_write_data.at(read++) == rfid_2.FIFODataReg);
	REQUIRE((int)mock_write_data.at(read++) == rfid_2.ControlReg);
	REQUIRE(mock_write_data.size() == read);
}

TEST_CASE("Init - ReadCardSerial", "[rfid_2]") {
	reset_for_test(i2c0);
	std::vector<uint8_t> readData;
	std::vector<uint8_t> expectedWriteData;

	// PICC_Select
	expectedWriteData.push_back(RFID_2::CollReg); //ReadRegister
	readData.push_back(0b00000000); // ClearRegisterBitMask - CollReg
	expectedWriteData.push_back(RFID_2::CollReg); //WriteRegister
	expectedWriteData.push_back(0);
	expectedWriteData.push_back(RFID_2::BitFramingReg);
	expectedWriteData.push_back(0);

	//PCD_CommunicateWithPICC
	expectedWriteData.push_back(RFID_2::CommandReg);
	expectedWriteData.push_back(RFID_2::PCD_Idle);
	expectedWriteData.push_back(RFID_2::ComIrqReg);
	expectedWriteData.push_back(0x7F);
	expectedWriteData.push_back(RFID_2::FIFOLevelReg);//ReadRegister
	readData.push_back(0b00000000); // SetRegisterBitMask - FIFOLevelReg
	expectedWriteData.push_back(RFID_2::FIFOLevelReg); //WriteRegister
	expectedWriteData.push_back(0x80); // FlushBuffer = 1, FIFO initialization
	expectedWriteData.push_back(RFID_2::FIFODataReg);
	expectedWriteData.push_back(147); //data
	expectedWriteData.push_back(32); //data
	expectedWriteData.push_back(RFID_2::BitFramingReg);
	expectedWriteData.push_back(0);
	expectedWriteData.push_back(RFID_2::CommandReg);
	expectedWriteData.push_back(RFID_2::ControlReg);
	expectedWriteData.push_back(RFID_2::BitFramingReg);//ReadRegister
	readData.push_back(0); //SetRegisterBitMask
	expectedWriteData.push_back(RFID_2::BitFramingReg);//WriteRegister
	expectedWriteData.push_back(0x80); // StartSend=1, transmission of data starts
	expectedWriteData.push_back(RFID_2::ComIrqReg);
	readData.push_back(0b00110000); // ComIrqReg - RxIRq and IdleIRq(0x30)
	expectedWriteData.push_back(RFID_2::ErrorReg);
	readData.push_back(0b00000000); // ErrorReg - no errors
	expectedWriteData.push_back(RFID_2::FIFOLevelReg); //Read bytes in FIFO
	readData.push_back(4); // FIFOLevelReg - number of bytes in the FIFO
	expectedWriteData.push_back(RFID_2::FIFODataReg);
	readData.push_back(0b00000000); // first uid byte
	readData.push_back(0b00000000); // second uid byte
	readData.push_back(0b00000000); // third uid byte
	readData.push_back(0b00000000); // fourth uid byte
	expectedWriteData.push_back(RFID_2::ControlReg);
	readData.push_back(0b00000000); // ControlReg - to check valid bits - 000b, the whole byte is valid

	// PCD_CalculateCRC
	expectedWriteData.push_back(RFID_2::CommandReg);
	expectedWriteData.push_back(RFID_2::PCD_Idle);
	expectedWriteData.push_back(RFID_2::DivIrqReg);
	expectedWriteData.push_back(0x04); // Clear the CRCIRq interrupt request bit
	expectedWriteData.push_back(RFID_2::FIFOLevelReg); // ReadRegister
	readData.push_back(0b00000000);
	expectedWriteData.push_back(RFID_2::FIFOLevelReg); // WriteRegister
	expectedWriteData.push_back(0x80); // FlushBuffer = 1, FIFO initialization

	expectedWriteData.push_back(RFID_2::FIFODataReg);
	expectedWriteData.push_back(147); // Data written to RFID_2::FIFODataReg
	expectedWriteData.push_back(112);
	expectedWriteData.push_back(0);
	expectedWriteData.push_back(0);
	expectedWriteData.push_back(0);
	expectedWriteData.push_back(0);
	expectedWriteData.push_back(0);

	expectedWriteData.push_back(RFID_2::CommandReg);
	expectedWriteData.push_back(RFID_2::PCD_CalcCRC);
	expectedWriteData.push_back(RFID_2::DivIrqReg);
	readData.push_back(0x04); // CRCIRq bit set - calculation done
	expectedWriteData.push_back(RFID_2::CommandReg);
	expectedWriteData.push_back(RFID_2::PCD_Idle);
	expectedWriteData.push_back(RFID_2::CRCResultRegL); //ReadRegister
	readData.push_back(0);
	expectedWriteData.push_back(RFID_2::CRCResultRegH); //ReadRegister
	readData.push_back(0);
	expectedWriteData.push_back(RFID_2::BitFramingReg);
	expectedWriteData.push_back(0);

	//PCD_CommunicateWithPICC
	expectedWriteData.push_back(RFID_2::CommandReg);
	expectedWriteData.push_back(RFID_2::PCD_Idle);
	expectedWriteData.push_back(RFID_2::ComIrqReg);
	expectedWriteData.push_back(0x7F);
	expectedWriteData.push_back(RFID_2::FIFOLevelReg);
	readData.push_back(0b00000000); // SetRegisterBitMask - FIFOLevelReg
	expectedWriteData.push_back(RFID_2::FIFOLevelReg);
	expectedWriteData.push_back(0x80);
	expectedWriteData.push_back(RFID_2::FIFODataReg);
	expectedWriteData.push_back(147); // Data written to RFID_2::FIFODataReg
	expectedWriteData.push_back(112);
	expectedWriteData.push_back(0);
	expectedWriteData.push_back(0);
	expectedWriteData.push_back(0);
	expectedWriteData.push_back(0);
	expectedWriteData.push_back(0);
	expectedWriteData.push_back(0);
	expectedWriteData.push_back(0);
	expectedWriteData.push_back(RFID_2::BitFramingReg);
	expectedWriteData.push_back(0);
	expectedWriteData.push_back(RFID_2::CommandReg);
	expectedWriteData.push_back(RFID_2::ControlReg);
	expectedWriteData.push_back(RFID_2::BitFramingReg);
	readData.push_back(0b00000000); // SetRegisterBitMask - BitFramingReg
	expectedWriteData.push_back(RFID_2::BitFramingReg);
	expectedWriteData.push_back(0x80);
	expectedWriteData.push_back(RFID_2::ComIrqReg);
	readData.push_back(0b00110000); // ComIrqReg - RxIRq and IdleIRq(0x30)
	expectedWriteData.push_back(RFID_2::ErrorReg);
	readData.push_back(0b00000000); // ErrorReg - no errors
	expectedWriteData.push_back(RFID_2::FIFOLevelReg); //Read bytes in FIFO
	readData.push_back(3); // FIFOLevelReg - number of bytes in the FIFO
	expectedWriteData.push_back(RFID_2::FIFODataReg);
	readData.push_back(0b00000000); // first uid byte
	readData.push_back(0b00000000); // second uid byte
	readData.push_back(0b00000000); // third uid byte
	expectedWriteData.push_back(RFID_2::ControlReg);
	readData.push_back(0b00000000); // ControlReg - to check valid bits - 000b, the whole byte is valid

	// PCD_CalculateCRC
	expectedWriteData.push_back(RFID_2::CommandReg);
	expectedWriteData.push_back(RFID_2::PCD_Idle);
	expectedWriteData.push_back(RFID_2::DivIrqReg);
	expectedWriteData.push_back(0x04); // Clear the CRCIRq interrupt request bit
	expectedWriteData.push_back(RFID_2::FIFOLevelReg); // ReadRegister
	readData.push_back(0b00000000);
	expectedWriteData.push_back(RFID_2::FIFOLevelReg); // WriteRegister
	expectedWriteData.push_back(0x80); // FlushBuffer = 1, FIFO initialization

	expectedWriteData.push_back(RFID_2::FIFODataReg);
	expectedWriteData.push_back(0);
	expectedWriteData.push_back(RFID_2::CommandReg);
	expectedWriteData.push_back(RFID_2::PCD_CalcCRC);
	expectedWriteData.push_back(RFID_2::DivIrqReg);
	readData.push_back(0x04); // CRCIRq bit set - calculation done
	expectedWriteData.push_back(RFID_2::CommandReg);
	expectedWriteData.push_back(RFID_2::PCD_Idle);
	expectedWriteData.push_back(RFID_2::CRCResultRegL); //ReadRegister
	readData.push_back(0);
	expectedWriteData.push_back(RFID_2::CRCResultRegH); //ReadRegister
	readData.push_back(0);
	expectedWriteData.push_back(RFID_2::BitFramingReg);
	expectedWriteData.push_back(0);

	set_read_data(readData);
	set_expected_write_data(expectedWriteData);

	bool readOk = rfid_2.PICC_ReadCardSerial();
	REQUIRE(readOk == true);
}
