# ForgetMeNot

<div>
<img src="https://github.com/ajebulon/ForgetMeNot/blob/master/circuit-off-medium.jpg" alt="drawing" width="300"/>
<img src="https://github.com/ajebulon/ForgetMeNot/blob/master/circuit-on-medium.jpg" alt="drawing" width="300"/>
</div>

DIY version of the Friendship lamps. With easy onboarding mechanism, when the lamp is turned on the first time, it will switch the ESP8266 module into AP-mode, and open an Access Point (AP), with SSID "ForgetMeNot_OB". You can connect to the Wi-Fi using the password specified in the source code. After connecting to the temporary AP, you can open the default webpage by accessing 192.168.1.184 from your device. It should open up the option to switch the ESP8266 module into STA-mode by supplying your home Wi-Fi SSID and password credential. After that, the lamp will reboot and automatically connect to your home Wi-Fi network using provided SSID and password.

You also need to register the MQTT broker by yourself. You can use the public MQTT broker, such as EMQX, as included in the source code. Just don't forget to use different topic from the source code.

| Item              | Description                              |
| :----:            |     :---                                 |
| <your_pub_topic>  | Topic to publish LED current state       |
| <your_sub_topic>  | Topic to receive LED on/off trigger      |
| <wifi_pw>         | Default password during first onboarding |

To control the LED using MQTT protocol, you can either use the GUI tools provided by your MQTT broker, or download any MQTT dashboard device for your operating system. For this example, I am using [MQTT Dash](https://play.google.com/store/apps/details?id=net.routix.mqttdash) for Android operating system. 

<div>
<img src="https://github.com/ajebulon/ForgetMeNot/blob/master/mqtt-dash.jpg" alt="drawing" height="400"/>
<img src="https://github.com/ajebulon/ForgetMeNot/blob/master/led.gif" alt="drawing" height="400"/>
</div>
