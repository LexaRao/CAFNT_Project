using UnityEngine;
using GameObject;

public class TransmiterScript : MonoBehaviour
{
    // Define the reciever for this program.
    public GameObject transmissionReceiver;
    public GameObject recievePhysicalRepresentation;
    int powerlevels = 0;
    bool signelPresent = false;

    // Start is called once before the first execution of Update after the MonoBehaviour is created
    void Start()
    {
        // Gather the game objects of both the transmittor and reciever.
        GameObject recievePos = transmissionReceiver.GetObject<GameObject>();
        GameObject transmissionPos = recievePhysicalRepresentation.GetObject<GameObject>();

        // Initialize both the transmittor and the reciever to off.
        recievePos.color = "Red";
        transmissionPos.color = "Red";
    }

    // Update is called once per frame
    void Update()
    {
        // When the transmittor is set to the right strength change the indicator for two ms.
        
        // Wait 4 ms before change the reciever and transmittor.
        // wait(4);

        // If power is correct level then allow the reciever to flicker.
        if (powerlevels >= 5) {
            // When the transmittor is able to recieve the data change it indicator light and transmitters.
            
            // Wait for ms.
            // wait(4);
            
            // Flip reciever back off.
            
        }

        // Change everything back to neteral before next single.

    }
}
