/*
 * \brief Source file for JbdBms class
 * \author rahmaevao
 * \version 0.1
 * \date September 2019
 */

#include "JbdBms.h"

void JbdBms::begin()
{
  m_serial.begin(9600);
}

bool JbdBms::readBmsData()
{
  uint8_t responce[BMS_LEN_RESPONCE];

  sendReqMessage();
  readResponce(responce);

  if (checkCheckSumRecieve(responce) == true)
  {
    parseTheMessage(responce);
  }
  else
  {
    return false;
  }
  return true;
}

float JbdBms::getCurrent()
{
  return m_current;
}

float JbdBms::getChargePercentage()
{
  return m_chargePercentage;
}
uint16_t JbdBms::getProtectionState()
{
  return m_protectionState;
}

void JbdBms::sendReqMessage()
{
  uint8_t reqMessage[] = {0xDDU, 0xA5, 0x03, 0x00, 0xFF, 0xFD, 0x77};
  m_serial.write(reqMessage, 7);
}

bool JbdBms::readResponce(uint8_t *t_outMessage)
{
  uint8_t i = 0;
  bool findBeginByte = false;
  uint32_t statrTime = millis();
  while (i <= BMS_LEN_RESPONCE - 1)
  {
    if (abs((millis() - statrTime) > getMaxTimeout()))
    {
      return false;
    }
    if (m_serial.available() > 0)
    {
      uint8_t thisByte = m_serial.read();
      if (thisByte == 0xDD)
      {
        findBeginByte = true;
      }
      if (findBeginByte)
      {
        t_outMessage[i] = thisByte;
        i++;
      }
    }
  }
  return true;
}

void JbdBms::parseTheMessage(uint8_t *t_message)
{
  m_current = ((t_message[6] << 8) | t_message[7]) * 10;
  m_chargePercentage = t_message[23];
  m_protectionState = (t_message[20] << 8) | t_message[21];
}

/**
 * \brief Compute and cheking check sum in message
 * \param [in] t_message
 * \return Fact of passing the test:
 *  true - check passed;
 *  false - check not passed
 */
bool JbdBms::checkCheckSumRecieve(uint8_t *t_message)
{

  uint16_t checkSumCompute;
  uint16_t checkSumAccepted;
  uint8_t lengthData;
  uint8_t startIndexCS;

  lengthData = t_message[3];
  checkSumCompute = computeCrc16JbdChina(t_message, BMS_LEN_RESPONCE);
  startIndexCS = lengthData + 4;

  checkSumAccepted = (t_message[startIndexCS] << 8) | t_message[startIndexCS + 1];

  if (checkSumCompute != checkSumAccepted)
    return false;
  return true;
}

/**
 * \brief Compute check sum in message for JBD protocol semiModbus :)
 * \param[in] puchMsg Message buffer containing binary data to be used for
 *            generating the CRC.
 * \param[in] usDataLen The quantity of bytes in the message buffer.
 * \return The function returns the CRC.
 */
uint16_t JbdBms::computeCrc16JbdChina(uint8_t *puchMsg, uint8_t usDataLen)
{
  uint8_t lengthData = puchMsg[3];

  uint16_t summa = 0;

  for (int i = 4; i < lengthData + 4; i++)
    summa = summa + puchMsg[i];

  uint16_t checkSum = (summa + lengthData - 1) ^ 0xFFFF;
  return checkSum;
}

/**
 * \brief   Сборка float из uint32_t
 */
float JbdBms::converUint32ToFloat(uint32_t number)
{
  union DataType
  {
    float f;
    uint32_t uint32t;
  };

  union DataType sample;
  sample.uint32t = number;
  return sample.f;
}

uint32_t JbdBms::getMaxTimeout()
{
  return 100;
}
