#include <stdio.h>
#include "esp_log.h"
#include "driver/i2c.h"
#include "sdkconfig.h"

#define DATA_LENGTH 128                  /*!< Data buffer length of test buffer */
#define RW_TEST_LENGTH 10                /*!< Data length for r/w test, [0,DATA_LENGTH] */

#define I2C_MASTER_SCL_IO (gpio_num_t)4              /*!< gpio number for I2C master clock */
#define I2C_MASTER_SDA_IO (gpio_num_t)5               /*!< gpio number for I2C master data  */
#define I2C_MASTER_FREQ_HZ 100000         /*!< I2C master clock frequency */
#define I2C_MASTER_TX_BUF_DISABLE 0                           /*!< I2C master doesn't need buffer */
#define I2C_MASTER_RX_BUF_DISABLE 0                           /*!< I2C master doesn't need buffer */

#define ESP_SLAVE_ADDR     0x28        /*!< ESP32 slave address, you can set any 7bit value */
#define WRITE_BIT     I2C_MASTER_WRITE          /*!< I2C master write */
#define READ_BIT      I2C_MASTER_READ           /*!< I2C master read */
#define ACK_CHECK_EN  0x1                       /*!< I2C master will check ack from slave*/
#define ACK_CHECK_DIS 0x0                       /*!< I2C master will not check ack from slave */
#define ACK_VAL       I2C_MASTER_ACK            /*!< I2C ack value */
#define NACK_VAL      I2C_MASTER_NACK           /*!< I2C nack value */

#define S2_SCL_GPIO 7
#define S2_SDA_GPIO 4

/**
 * @brief test code to read esp-i2c-slave
 *        We need to fill the buffer of esp slave device, then master can read them out.
 *
 * _______________________________________________________________________________________
 * | start | slave_addr + rd_bit +ack | read n-1 bytes + ack | read 1 byte + nack | stop |
 * --------|--------------------------|----------------------|--------------------|------|
 *
 */
static esp_err_t i2c_master_read_slave(i2c_port_t i2c_num, uint8_t *data_rd, size_t nsize)
{
    if (nsize == 0) {
        return ESP_OK;
    }
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (ESP_SLAVE_ADDR << 1) | READ_BIT, ACK_CHECK_EN); 
    if (nsize > 1) {
        i2c_master_read(cmd, data_rd, nsize - 1, ACK_VAL);
    }
    i2c_master_read_byte(cmd, data_rd + nsize - 1, NACK_VAL);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(i2c_num, cmd, 1000 / portTICK_RATE_MS); // send all queued commands
    i2c_cmd_link_delete(cmd);
    return ret;
}

/**
 * @brief Test code to write esp-i2c-slave
 *        Master device write data to slave(both esp32),
 *        the data will be stored in slave buffer.
 *        We can read them out from slave buffer.
 *
 * ___________________________________________________________________
 * | start | slave_addr + wr_bit + ack | write n bytes + ack  | stop |
 * --------|---------------------------|----------------------|------|
 *
 */
static esp_err_t i2c_master_write_slave(i2c_port_t i2c_num, uint8_t *data_wr, size_t nsize)
{
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (ESP_SLAVE_ADDR << 1) | WRITE_BIT, ACK_CHECK_EN);
    i2c_master_write(cmd, data_wr, nsize, ACK_CHECK_EN);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(i2c_num, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    return ret;
}


/**
 * @brief i2c master initialization
 */
static esp_err_t i2c_master_init()
{
    i2c_port_t i2c_master_port = I2C_NUM_0;
    i2c_config_t conf;
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = S2_SDA_GPIO;                //I2C_MASTER_SDA_IO;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_io_num = S2_SCL_GPIO;                //I2C_MASTER_SCL_IO;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = I2C_MASTER_FREQ_HZ;
    i2c_param_config(i2c_master_port, &conf);
    return i2c_driver_install(i2c_master_port, conf.mode,
                              I2C_MASTER_RX_BUF_DISABLE,
                              I2C_MASTER_TX_BUF_DISABLE, 0);
}


typedef struct MotorControlMessage {
    char cDir;
    int nLeftPowerPercent;
    int nRightPowerPercent;
} MotorControlMessage;

MotorControlMessage sentData;

void SendI2CData()
{
  sentData.nLeftPowerPercent = 0.75*sentData.nLeftPowerPercent;
  if (i2c_master_write_slave(I2C_NUM_0, (uint8_t*)&sentData, sizeof(sentData)) == ESP_OK)
      Serial.printf("Sent to slave: %c direction, %d left power, %d right power\n",sentData.cDir, sentData.nLeftPowerPercent, sentData.nRightPowerPercent);
}
