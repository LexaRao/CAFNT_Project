using UnityEngine; // Required for MonoBehaviour, GameObject, Color, and Unity API
using System.IO; // Required for FileStream, StreamReader, and file I/O
using System.Collections; // Required for IEnumerator and coroutines

public class TransmiterScript : MonoBehaviour
{
    // Define the receiver for this program.
    public GameObject transmissionReceiver;
    public GameObject receivePhysicalRepresentation;
    int powerlevels = 0;
    bool signalPresent = false;
    FileStream fileStream;
    StreamReader streamReader;
    private GameObject receivePos;
    private GameObject transmissionPos;
    private Renderer receiveRenderer;
    private Renderer transmissionRenderer;
    private bool isProcessing = false;
    private string dataSubset = string.Empty;

    // Start is called once before the first execution of Update after the MonoBehaviour is created
    void Start()
    {
        // Gather the game objects of both the transmittor and receiver.
        if (transmissionReceiver == null)
        {
            transmissionReceiver = GameObject.Find("Receiver");
        }

        if (receivePhysicalRepresentation == null)
        {
            receivePhysicalRepresentation = GameObject.Find("Transmitter");
        }

        receivePos = transmissionReceiver;
        transmissionPos = receivePhysicalRepresentation;

        if (receivePos != null)
        {
            receiveRenderer = receivePos.GetComponent<Renderer>();
        }

        if (transmissionPos != null)
        {
            transmissionRenderer = transmissionPos.GetComponent<Renderer>();
        }

        // Initialize both the transmittor and the receiver to off.
        SetObjectColor(receiveRenderer, Color.red);
        SetObjectColor(transmissionRenderer, Color.red);

        // Input the correct fiels belonging to this repository.
        string filePath = Path.Combine(Application.dataPath, "Files/keyTracker.txt.ink");
        if (File.Exists(filePath))
        {
            fileStream = new FileStream(filePath, FileMode.Open, FileAccess.Read);

            // Access the data to determine if the stream has had the correct data sent to it.
            streamReader = new StreamReader(fileStream);
            string data = streamReader.ReadToEnd();
            streamReader.Close();
            fileStream.Close();

            // Gather subsection of the data for the program.
            dataSubset = data.Length >= 18 ? data.Substring(4, 14) : string.Empty;

            // If the data is correct then set the power levels to 5 and the signel present to true.
            if (dataSubset == "Pressed")
            {
                powerlevels = 6;
                signalPresent = true;
            }
        }
        else
        {
            Debug.LogWarning($"Missing file: {filePath}");
        }
    }

    // Update is called once per frame
    void Update()
    {
        // When the transmittor is set to the right strength change the indicator for two ms.
        if (signalPresent && !isProcessing)
        {
            StartCoroutine(ProcessTransmission());
        }
    }

    private IEnumerator ProcessTransmission()
    {
        isProcessing = true;

        // Change the transmittor indicator to green.
        SetObjectColor(transmissionRenderer, Color.green);

        // Wait 4 ms before change the reciever and transmittor.
        yield return new WaitForSeconds(0.004f);

        // If power is correct level then allow the receiver to flicker.
        if (powerlevels >= 5)
        {
            // When the transmittor is able to receive the data change it indicator light and transmitters.
            SetObjectColor(transmissionRenderer, Color.red);
            SetObjectColor(receiveRenderer, Color.green);

            // Wait for ms.
            yield return new WaitForSeconds(0.004f);
            
            // Flip reciever back off.
            SetObjectColor(receiveRenderer, Color.red);
        }

        // Change everything back to neteral before next single.
        SetObjectColor(transmissionRenderer, Color.red);
        SetObjectColor(receiveRenderer, Color.red);

        // Gather next segment of the data.
        string filePath = Path.Combine(Application.dataPath, "Files/keyTracker.txt.ink");
        if (File.Exists(filePath))
        {
            string data = File.ReadAllText(filePath);
            dataSubset = data.Length >= 18 ? data.Substring(4, 14) : string.Empty;

            // If the data is correct then set the power levels to 5 and the signel present to true.
            if (dataSubset == "Pressed")
            {
                powerlevels = 6;
                signalPresent = true;
            }
            else
            {
                powerlevels = 0;
                signalPresent = false;
            }
        }
        else
        {
            Debug.LogWarning($"Missing file: {filePath}");
            signalPresent = false;
        }

        isProcessing = false;
        yield return null;
    }

    private void SetObjectColor(Renderer renderer, Color color)
    {
        if (renderer != null)
        {
            renderer.material.color = color;
        }
    }
}
