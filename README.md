# CS 578 CAFNT_Project

Team member: Lexa Hope, 
             Heidi Lin, 828595229
             
Brief introduction of the project: A common api for network tunneling over long distances using IR and networks.

1. System Architecture:
   - PC Side (Serial Daemon)
     Runs on a Windows machine
     Communicates over a serial port (COM3)
     Accepts user commands via terminal
     Sends commands to the Arduino
     Receives and prints responses
   - Arduino Side (IR Receiver Module)
     Receives commands via serial
     Executes hardware-level operations
     Handles IR remote input
     Controls LEDs and buzzer
    - Flow: PC => USB (optional) => Arduino(IR Receiver)
      
2. Serial Command API (defines how the PC communicates with the Arduino):
   - Command Format:
     power_on
     set_level 5
     set_color blue
     status
   - Response Format:
     OK POWER ON
     OK LEVEL 5
     OK COLOR BLUE
     POWER=ON;LEVEL=5;COLOR=1;ALERT=true
    - Supported Commands:

      
Command     | 	Parameters   | 	Description      |	Expected Behavior

=====================================================================================
power_on	  | none	         | Turn system on    |	Enables device

power_off	  | none	         | Turn system off   |	Disables device

power_toggle| none	         | Toggle power state|	Switches ON/OFF

set_level	  | 0–9	           | Set power level	 | Updates internal level

get_level	  | none	         | Get power level	 | Returns current level

get_power	  | none	         | Get power state	 | Returns ON/OFF

set_color   | red/blue/green | Set LED color	   | Changes LED output

led_off     |	none           | Turn LED off	     | Disables LED

beep	      | none	         | Play normal beep	 | Buzzer feedback

invalid_beep|	none	         | Play error beep	 | Error feedback

dnd_on	    | none	         | Disable alerts	   | Enable do-not-disturb

dnd_off     | none           | Enable alerts     | Disable do-not-disturb

status	    | none	         | Get system status | Returns full system state


 3. Arduino Function Mapping (executed on the Arduino when commands are received):
    Command        |   Arduino Function
    -------------------------------------------
    power_on	     |   changePowerState(true)
    power_off	     |   changePowerState(false)
    set_level	     |   changePLRec(level)
    get_level	     |   moniterPowerLevel()
    get_power	     |   recIsPowered()
    set_color red	 |   changeColor(0) + writeImmRed()
    set_color blue |   changeColor(1) + writeImmBlue()
    set_color green|   changeColor(2) + writeImmGreen()
    led_off	       |   writeImmOff()
    beep           |  	createBEEP()
    invalid_beep   |  	CreateInvalidBEEP()
    dnd_on	       |    doNotDisturb(false)
    dnd_off	       |    doNotDisturb(true)

  4. IR Remote Integration Mapping(receive IR remote input and translate it into internal commands)
     Mode Code |    Remote Button    |  Action
     ----------------------------------------------------------------------
     0	       | POWER	             | Toggle system power
     1	       | FUNC/STOP	         | Beep + Blue LED
     2	       | VOL+	               | Increase power level
     3	       | FAST BACK	         | Flash Red LED
     4	       | PAUSE	             | Flash Blue LED
     5	       | FAST FORWARD	       | Flash Green LED
     6	       | DOWN                | Disable alerts (DND ON)
     7	       | VOL-	               | Decrease power level
     8	       | UP	                 | Enable alerts (DND OFF)
     9	       | EQ	                 | Normal beep
     10	       | ST/REPT	           | Invalid/error beep
     11–20	   | Number buttons (0–9)| Set power level directly
     21        | Other	             | Unknown input handling
     
     - Internal Processing Flow:
       IR signal is received
       translateIR() converts the signal into a mode code
       modeSwitch(modeCode) stores the selected mode
       pushSwitch(modeCode) executes the corresponding action
       
  6. Local Integration Requirements: 
     - PC Side: Send serial commands using WriteFile (Windows API), and match commands defined.
     - Arduino Side: Read serial input using Serial.readStringUntil('\n'), parse commands, then call corresponding functions. 
     
    
    
    
 
     
     
     
   
