examples/bluetooth

^^^^^^^^^^^^^^^^^^^^



  This is simple example of the MK71251 bluetooth module. Available as nsh built-in command.

  This example starts bluetooth device advertising and waits for connection.
  When connection is available,
  all data sent to devicve is shown and
  if character 'Z' is received it will reply with "WRITE TEST". 


  Application will run until board is reset.



  Supported sensor is Lapis MK71251.



  This application depends on its device.



  Configuration Pre-requisites:



    CONFIG_BH1745NUC - BH1745NUC




  Example Configuration:



    CONFIG_EXAMPLES_BLE - Enable this example

    CONFIG_EXAMPLES_BLE_PROGNAME - Program name.

    CONFIG_EXAMPLES_BLE_PRIORITY - Example priority. Default: 100

    CONFIG_EXAMPLES_BLE_STACKSIZE - Example stack size. Default: 2048



  Operation:



    No console operation is needed.
    You need to download the LAPIS BLE tool for your phone to test the module.
    Open bluetooth host application and connect to 'LapisDev' device.
    Send data from application and you will see it on console. Sending character
    'Z' will get reply string "WRITE TEST"

