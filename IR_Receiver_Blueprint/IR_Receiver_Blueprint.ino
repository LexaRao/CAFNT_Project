//www.elegoo.com
//2023.05.05

//SDSU: www.sdsu.edu.
//2026.03.08

// Include required libraries.
#include "src/IRremote.h"

// Define public vars.  
int receiver = 11; // Signal Pin of IR receiver to Arduino Digital Pin 11
int level = 0;
int curColor = 0;

// Define the variables for the program at the reciever to function.
// Receiver power variables.
bool recPowered = false;
bool alertUser = false;

// Define the current system mode.
int currentMode;

// Define the pin out based on the mode.
#define Beep 8
#define Blue 3
#define Green 5
#define Red 6

// When the users does something invalid alert them.
void CreateInvalidBEEP() {
  // Define needed variables.
  const unsigned long smallDelay = 50;

  // Beep three times to allow the user to understand something has gone wrong.
  // First time.
  digitalWrite(Beep, HIGH);
  delay(smallDelay);
  digitalWrite(Beep, LOW);

  // Second time.
  digitalWrite(Beep, HIGH);
  delay(smallDelay);
  digitalWrite(Beep, LOW);

  // Third time.
  digitalWrite(Beep, HIGH);
  delay(smallDelay);
  digitalWrite(Beep, LOW);
}

// Define the function required in this program.
// Create a function to turn all lights off with or without signel.
void writeImmOff() {
  // Turn off all lights.
  digitalWrite(Red, LOW);
  digitalWrite(Green, LOW);
  digitalWrite(Blue, LOW);
}

// Create a function for the red color.
void writeRed() {
  // If the receiver is powered and at the write power level write the function.
  if (recPowered && level >= 5) {
    digitalWrite(Red, HIGH);
    digitalWrite(Green, LOW);
    digitalWrite(Blue, LOW);

    // Wait a couple of seconds before turning off the light.
    delay(1000);
    writeImmOff();
  } else if (alertUser) {
    CreateInvalidBEEP();
  } 
}

// Create a function for the green color.
void writeGreen() {
  // If the reciever is powered and at the right power level write the color green.
  if (recPowered && level >= 5) {
    digitalWrite(Green, HIGH);
    digitalWrite(Blue, LOW);
    digitalWrite(Red, LOW);

    // Wait a couple of seconds before turning off the light.
    delay(1000);
    writeImmOff();
  } else if (alertUser) {
    CreateInvalidBEEP();
  }
}

// Create a function for the color blue.
void writeBlue() {
  // If the receiver is powered and at the right power level write the color blue.
  if (recPowered && level >= 5) {
    digitalWrite(Blue, HIGH);
    digitalWrite(Green, LOW);
    digitalWrite(Red, LOW);

    // Wait a couple of seconds before turning off the light.
    delay(1000);
    writeImmOff();
  } else if (alertUser) {
    CreateInvalidBEEP();
  }
}

// Create a set of functions for writing the color immidently.
// Create for the color blue.
void writeImmBlue() {
  digitalWrite(Blue, HIGH);
  digitalWrite(Green, LOW);
  digitalWrite(Red, LOW);
}

// Create for the color red.
void writeImmRed() {
  digitalWrite(Red, HIGH);
  digitalWrite(Green, LOW);
  digitalWrite(Blue, LOW);
}

// Create for the color green.
void writeImmGreen() {
  digitalWrite(Green, HIGH);
  digitalWrite(Blue, LOW);
  digitalWrite(Red, LOW);
}

// Create a function for beeping for a total of 4 seconds.
void createBEEP() {
  // If the receiver is powered and at the rigth power level beep.
  if (recPowered && level >= 5) {
    // Make a beep for 4000 ms.  
    digitalWrite(Beep, HIGH);
    delay(40);
    digitalWrite(Beep, LOW);

  } else if (alertUser) {
    CreateInvalidBEEP();
  }
}

// Create a function for changing the power level at the receiver.
void changePLRec(int powerLevel) {
  // Change the local level with the power level.
  level = powerLevel;
}

// Moniter the power level at the transmitter.
int moniterPowerLevel() {
  return level;
}

// Get the state of the receiver if it is powered or not.
bool recIsPowered() {
  return recPowered;
}

// Change the power state at the receiver.
void changePowerState(bool powerState) {
  recPowered = powerState;
}

// Alert users upon required need.
bool alertUserPowerDown() {
  return alertUser;
}

// Change the start of userAlert also known as the do not disturb flag.
bool doNotDisturb(bool notificationOn) {
  alertUser = notificationOn;
}

// Create a function for changing the color.
// Red: 0.
// Blue: 1.
// Green: 2.
void changeColor(int color) {
  curColor = color;
}

// Return the current color.
int getColor() {
  // Return the current color.
  return curColor;
}

// Declare the function for storing the current mode.
void modeSwitch(int modeCode) {
  currentMode = modeCode;
}

// Get the current mode in the switch.
int getSwitch() {
  // Return the current mode.
  return currentMode;
}

// Connection management.
// Declare a function for being able to parse each of the cases in order.
void pushSwitch(int modeCode) {
  // Parse and act for each mode.
  // Create the function for when the power button is hit.
  if (modeCode == 0) {
    // Indicate to the user what is happening.
    Serial.println("POWER");

    // Press the power button when this is happening.
    if (recIsPowered() == true) {
      // Power off the receive if it is already powered.
      changePowerState(false);
    } else {
      // Power on the receiver.
      changePowerState(true);
    }

  // Create the function for when the stop function is hit.
  } else if (modeCode == 1) { 
    // Print the output of the function to the user.
    Serial.println("FUNC/STOP"); 

    // Create a beep for user interaction.
    createBEEP();

    // Change the color of the light to blue without waiting.
    writeImmBlue();

  // Create command for turning up volume.
  } else if (modeCode == 2) {
    // Create the volume up command for change the strength of the transmitter.
    Serial.println("VOL+");

    // Get the current strength of the transmitter.
    int powerLevel = moniterPowerLevel();

    // Add on value to the power level.
    powerLevel++;

    // Store this power level in the transmitter.
    changePowerState(powerLevel);

    // Create beep due to the change in the power level.
    createBEEP();

  // Create a function for change the light color to red.  
  } else if (modeCode == 3) {
    // Write a message to computer about current button.
    Serial.println("FAST BACK");    

    // Change the color level to red.
    writeRed();

    // Create beep upon write.
    createBEEP();

  // Create a function for change color blue.
  } else if (modeCode == 4) {
    // Write the button to the users if it is pressed.
    Serial.println("PAUSE");    

    // Change the color of the level to blue.
    writeBlue();

    // Create the beep upon write.
    createBEEP();
    
  // Create the mode code for writing green.
  } else if (modeCode == 5) {
    // Write a users message to the serial about the current button.
    Serial.println("FAST FORWARD");   

    // Change the color at this case to green for the button.
    writeGreen();

    // Create beep upon write.
    createBEEP();

  // Create the mode code for setting function on do not rebound.
  } else if (modeCode == 6) {
    // Alert the users the button has been pressed.
    Serial.println("DOWN");  

    // Do not alert the user upon power down.
    doNotDisturb(false);

    // Create a beep upon received message.
    createBEEP();

  // Create the mode code for setting the current power level down.
  } else if (modeCode == 7) {
    // Alert the user the levels of the receiver are changing.
    Serial.println("VOL-");    

    // Get the current level within the program
    int currentLevel = moniterPowerLevel();

    // Decrease the levels by 1.
    currentLevel--;

    // Set the current levels based on this.
    changePLRec(currentLevel);

  // Create the mode code for turning rebound off.
  } else if (modeCode == 8) {

    // Alert the users the do not disturb is lifted.
    Serial.println("UP");    

    // When the user lifts the do not disturb allow for notifications.
    doNotDisturb(true);

    // Create beep when this command goes through.
    createBEEP();

  // Test the beep noise.
  } else if (modeCode == 9) {
    // Alert user this button has been pressed.
    Serial.println("EQ"); 

    // Demonstrate the beep function.
    createBEEP();

  // Test the function of the invalid beep noise.
  } else if (modeCode == 10) {
    // Alert the user when this button has been pressed.
    Serial.println("ST/REPT");    

    // Create a invalid beep when this button has been pressed.  
    CreateInvalidBEEP();

  // Test the function of the level zero power at receiver.
  } else if (modeCode == 11) {
    // Set the level in the function to zero. 
    Serial.println("0"); 

    // Set the level for the power.
    changePLRec(0);   

    // Create a beep to alert the users.
    createBEEP();

  // Test the function of the level one power at receiver.  
  } else if (modeCode == 12) {
    // Output the current pressed button for the users.
    Serial.println("1");  

    // Set the current power levels.
    changePLRec(1);

    // Create a beep to alert the users.
    createBEEP();  

  // Test the function of the level two power at receiver.  
  } else if (modeCode == 13) {
    // Create a printed line to the output.
    Serial.println("2");    

    // Set the current power levels for the function.
    changePLRec(2);

    // Create a beep to alert the users.
    createBEEP();

  // Test the functionality of the level three power level at the receiver. 
  } else if (modeCode == 14) {
    // Create output for user to understand power level.
    Serial.println("3");  

    // Set the current power level for this function.
    changePLRec(3);

    // Create a beep to alert the user.
    createBEEP();

  // Test the functionality of the level at power level four at the receiver.  
  } else if (modeCode == 15) {
    // Create the output for the users to understand the current power level.
    Serial.println("4");    

    // Set the current power level for this function.
    changePLRec(4);

    // Create a beep to alert the users.
    createBEEP();

  // Test the functionality of power level five at the receiver.  
  } else if (modeCode == 16) {
    // Create the output for the users to understand the current power level.
    Serial.println("5");  

    // Set the current power level for this function.
    changePLRec(5);

    // Create a beep to alert the users.
    createBEEP();

  // Test the functionality of the power level at level six of the receiver.  
  } else if (modeCode == 17) {
    // Create the output for the users to understand the current power levels.
    Serial.println("6"); 

    // Set the current power level for this function.
    changePLRec(6);

    // Create the beep to alert the users.
    createBEEP(); 

    // Test the functionality of the power level at level seven of the receiver.
  } else if (modeCode == 18) {
    // Create the output to the serial port to alert the users of the power level change. 
    Serial.println("7");    

    // Set the current power level for this function.
    changePLRec(7);

    // Create a beep to alert the users of this change.
    createBEEP();
  
  // Create a functionality for the power level at the level eight of the receiver.
  } else if (modeCode == 19) {
    // Create output to allow user to understand change in power levels.
    Serial.println("8");  

    // Set the current power levels for this function.
    changePLRec(8);

    // Create a beep to alert the users of this change.
    createBEEP();

  // Create the functionality for the power level at  the level nine of the receiver.
  } else if (modeCode == 20) {
    // Create a output to allow the user to understand the change in power levels.
    Serial.println("9"); 

    // Set the current power levels for the function.
    changePLRec(9);

    // Create a beep to alert the users of this change.
    createBEEP();

  // Otherwise create the default case.
  } else {
    // If other button is pressed alert the users.  
    Serial.println(" other button   ");
  }
}

// Required functionality of the API interface for commicated between different devices.
// Core functionality.
/* Network Configuration */

/* Devices & Network Infomation */
// Define the amount of supported connection to the board.
int supportedConnection() {
  return 1; // Right now the board only supports one connection to the board at a time.
}

/*-----( Declare objects )-----*/
IRrecv irrecv(receiver);     // create instance of 'irrecv'
//vairable uses to store the last decodedRawData
uint32_t last_decodedRawData = 0;
/*-----( Function )-----*/
void translateIR() // takes action based on IR code received
{
  // Check if it is a repeat IR code 
  if (irrecv.decodedIRData.flags)
  {
    //set the current decodedRawData to the last decodedRawData 
    irrecv.decodedIRData.decodedRawData = last_decodedRawData;
    Serial.println("REPEAT!");
  } else
  {
    //output the IR code on the serial monitor
    Serial.print("IR code:0x");
    Serial.println(irrecv.decodedIRData.decodedRawData, HEX);
  }

  // Date transmission.
  //map the IR code to the remote key
  switch (irrecv.decodedIRData.decodedRawData)
  {
    case 0xBA45FF00: 
      // Behave according to the functionality for the first case.
      modeSwitch(0);

      // Breakpoint in program.
      break;

    case 0xB847FF00: 
      // Behave according to the functionality of the second case.
      modeSwitch(1);

      // Breakpoint in the code.
      break;

    case 0xB946FF00: 
      // Create the volume up command for change the strength of the transmitter using mode switch.
      modeSwitch(2);

      // Break the program when it gets to this case.
      break;

    case 0xBB44FF00: 
      // Write a message to computer about current button using mode switch.
      modeSwitch(3);

      // Break at end of line.
      break;

    case 0xBF40FF00: 
      // Write the button to the users if it is pressed using mode switch.
      modeSwitch(4);      

      // Break the case at this line.
      break;

    case 0xBC43FF00: 
      // Write a users message to the serial about the current button using mode switch.
      modeSwitch(5);
    
      // Break the case when the program get to this point.
      break;

    case 0xF807FF00: 
      // Alert the users the button has been pressed using mode switch.
      modeSwitch(6);

      // Break when the case get to this point.
      break;

    case 0xEA15FF00: 
      // Create the code for the function when the user hits the volume down button.
      modeSwitch(7);

      // Break the program when it get to this point.
      break;

    case 0xF609FF00: 
      // Create the code for the users when the user hits the up button.
      modeSwitch(8);

      // Break the program when it get to this point.
      break;

    case 0xE619FF00: 
      // Create the code for the users when the user hits the EQ button.
      modeSwitch(9);

      // Break case at this position.   
      break;

    case 0xF20DFF00:
      // Using the mode code for the users when the user hits the ST/REPT button.
      modeSwitch(10);

      // Break point for this case.
      break;

    case 0xE916FF00:
      // Access the function for when the users hits the power level 0 button.
      modeSwitch(11);

      // Break point for this case.
      break;

    case 0xF30CFF00: 
      // Access the functionality for the program when the user hits the power level 1 button
      modeSwitch(12);

      // Break point for this case.
      break;

    case 0xE718FF00: 
      // Access the functionality for the program when the users hits the power level 2 button.
      modeSwitch(13);

      // Break point for this case.
      break;

    case 0xA15EFF00: 
      // Access the functionality for the program when the users hits the power level 3 button.
      modeSwitch(14);

      // Break point at this point in the program.  
      break;

    case 0xF708FF00: 
      // Access the functionality for the program when the users hits the program level 4 button.
      modeSwitch(15);

      // Break at this point in the case.
      break;

    case 0xE31CFF00: 
      // Access the functionality for the program when the users hits the program level 5 button.
      modeSwitch(16);

      // Break at this point in the case.  
      break;

    case 0xA55AFF00: 
      // Access the functionality for the program when the users hits the program level 6 button. 
      modeSwitch(17);

      // Break at this point in the case.  
      break;

    case 0xBD42FF00:
      // Access the functionality for the program when the users hits the program level 7 button.
      modeSwitch(18);

      // If the function get to this point break the function.
      break;

    case 0xAD52FF00: 
      // Access the functionality for the program when the users hits the program level 8 button.
      modeSwitch(19);

      // If the function get to this point break the function.  
      break;

    case 0xB54AFF00: 
      // Access the functionality for the program when the users hits the program level 9 button.  
      modeSwitch(20);

      // If the function get tot his point break the function.   
      break;

    default:
      // If other button is pressed alert the users using mode switch.
      modeSwitch(21); 
      
  }// End Case
  //store the last decodedRawData
  last_decodedRawData = irrecv.decodedIRData.decodedRawData;
  delay(500); // Do not get immediate repeat
} //END translateIR

void setup()   /*----( SETUP: RUNS ONCE )----*/
{
  // Create the pinout function.
  pinMode(Red, OUTPUT);
  pinMode(Green, OUTPUT);
  pinMode(Blue, OUTPUT);
  pinMode(Beep, OUTPUT);

  // Make sure to initilize the IR reciever.
  Serial.begin(9600);
  Serial.println("IR Receiver Button Decode");
  irrecv.enableIRIn(); // Start the receiver

  // Set the LED to red by default.
  writeImmRed();
  digitalWrite(Beep, HIGH);
  delay(1000);
  writeImmOff();
  digitalWrite(Beep, LOW);

  // Beep once to alert the users to the transmitter is up.
  // tone(Beep, 82, 10);
}/*--(end setup )---*/


void loop()   /*----( LOOP: RUNS CONSTANTLY )----*/
{
  if (irrecv.decode()) // have we received an IR signal?
  {
    translateIR();

    // Based on the current translation execute the current mode.
    int selectedMode = getSwitch();
    pushSwitch(selectedMode);

    irrecv.resume(); // receive the next value
  }  
}/* --(end main loop )-- */
