using System.Collections; // Required for IEnumerator and coroutines
using System.IO; // Required for file reading and writing
using UnityEngine; // Required for Unity types and API

public class TransmissionScript : MonoBehaviour // Class name matches file name and inherits MonoBehaviour
{
    public GameObject transmissionReceiver; // Reference to the receiver GameObject
    public GameObject receivePhysicalRepresentation; // Reference to the transmitter GameObject
    public string filePath = "Files/keyTracker.txt.ink"; // Relative file path under Assets

    private int powerLevels = 0; // Tracks the current power level
    private bool signalPresent = false; // Indicates whether a signal was detected
    private bool isProcessing = false; // Prevents re-entering the coroutine while processing
    private Renderer receiverRenderer; // Cached renderer for the receiver object
    private Renderer transmitterRenderer; // Cached renderer for the transmitter object

    void Awake() // Called when the script instance is initialized
    {
        if (transmissionReceiver == null) // Try fallback lookup if no object assigned
        {
            transmissionReceiver = GameObject.Find("Receiver"); // Find object by name
            if (transmissionReceiver == null) // Warn if still missing
            {
                Debug.LogWarning("TransmissionScript: transmissionReceiver is not assigned and no GameObject named 'Receiver' was found.");
            }
        }

        if (transmissionReceiver != null) // Cache the renderer if object exists
        {
            receiverRenderer = transmissionReceiver.GetComponent<Renderer>(); // Get renderer component
        }

        if (receivePhysicalRepresentation == null) // Try fallback lookup if no object assigned
        {
            receivePhysicalRepresentation = GameObject.Find("Transmitter"); // Find object by name
            if (receivePhysicalRepresentation == null) // Warn if still missing
            {
                Debug.LogWarning("TransmissionScript: receivePhysicalRepresentation is not assigned and no GameObject named 'Transmitter' was found.");
            }
        }

        if (receivePhysicalRepresentation != null) // Cache the renderer if object exists
        {
            transmitterRenderer = receivePhysicalRepresentation.GetComponent<Renderer>(); // Get renderer component
        }
    }

    void Start() // Called before the first frame update
    {
        SetObjectColor(receiverRenderer, Color.red); // Initialize receiver indicator to red
        SetObjectColor(transmitterRenderer, Color.red); // Initialize transmitter indicator to red
        ReadSignalFromFile(); // Read initial signal state from file
    }

    void Update() // Called once per frame
    {
        if (signalPresent && !isProcessing) // Start transmission only when a signal is present and not already processing
        {
            StartCoroutine(ProcessTransmission()); // Begin the transmission coroutine
        }
    }

    private void ReadSignalFromFile() // Reads the external signal file and updates state
    {
        string fullPath = Path.Combine(Application.dataPath, filePath); // Build absolute path to the file
        if (!File.Exists(fullPath)) // Check file existence before reading
        {
            Debug.LogWarning($"Signal file not found: {fullPath}"); // Warn if missing
            return; // Exit early if file is not available
        }

        string data = File.ReadAllText(fullPath); // Read the entire file contents
        string dataSubset = data.Length >= 18 ? data.Substring(4, 14) : string.Empty; // Extract the required substring safely

        if (dataSubset == "Pressed") // Detect the correct signal text
        {
            powerLevels = 6; // Set power level when pressed is found
            signalPresent = true; // Mark that a signal is present
        }
        else
        {
            powerLevels = 0; // Reset power level when signal is not present
            signalPresent = false; // Mark that no signal is present
        }
    }

    private IEnumerator ProcessTransmission() // Handles the visual transmission sequence and file update
    {
        isProcessing = true; // Prevent overlapping processing cycles

        SetObjectColor(transmitterRenderer, Color.green); // Turn transmitter indicator green
        yield return new WaitForSeconds(0.004f); // Wait briefly before continuing

        if (powerLevels >= 5) // If there is enough power, show receiver activity
        {
            SetObjectColor(transmitterRenderer, Color.red); // Reset transmitter indicator to red
            SetObjectColor(receiverRenderer, Color.green); // Turn receiver indicator green
            yield return new WaitForSeconds(0.004f); // Wait briefly while visible
            SetObjectColor(receiverRenderer, Color.red); // Reset receiver indicator to red
        }

        SetObjectColor(transmitterRenderer, Color.red); // Ensure transmitter ends red
        SetObjectColor(receiverRenderer, Color.red); // Ensure receiver ends red

        string fullPath = Path.Combine(Application.dataPath, filePath); // Resolve full file path again for writing
        try
        {
            File.WriteAllText(fullPath, "Key=Notpressed"); // Overwrite the file to clear the signal
        }
        catch (IOException e) // Handle I/O failures gracefully
        {
            Debug.LogError($"Failed to write signal file: {e.Message}"); // Log any write errors
        }

        signalPresent = false; // Clear the signal presence state
        yield return new WaitForSeconds(0.5f); // Wait half a second before reading again

        ReadSignalFromFile(); // Refresh signal state from file
        isProcessing = false; // Allow future processing cycles
    }

    private void SetObjectColor(Renderer renderer, Color color) // Sets the renderer material color if available
    {
        if (renderer != null) // Only attempt to set color when a renderer exists
        {
            renderer.material.color = color; // Apply the color to the material
        }
    }
}
