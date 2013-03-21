package voice.commands;

import edu.cmu.sphinx.frontend.util.Microphone;
import edu.cmu.sphinx.recognizer.Recognizer;
import edu.cmu.sphinx.result.*;
import edu.cmu.sphinx.decoder.search.Token;
import edu.cmu.sphinx.util.props.ConfigurationManager;

import java.io.*;
import java.util.*;
import java.text.DecimalFormat;

// Since json
import java.util.Scanner;
import java.util.NoSuchElementException;
import com.google.gson.Gson;

/* XML Parsing */
import org.w3c.dom.*;
import org.xml.sax.SAXException;
import javax.xml.parsers.*;
import javax.xml.xpath.*;

/**
 * A simple speech application built using Sphinx-4. This application uses the Sphinx-4
 * endpointer, which automatically segments incoming audio into utterances and silences.
 * It recognizes configured voice commands and triggers associated actions.
 */
public class VoiceCommands {

    private static DecimalFormat format = new DecimalFormat("#.#####");
    private static String config_commands = "commands.json";
    public static Thread t;

    public static void main(String[] args) {
        System.out.print("Loading Sphinx config ... ");
        /* Load Sphinx Configuration */
        ConfigurationManager cm = new ConfigurationManager(VoiceCommands.class.getResource("sphinx.config.xml"));
        System.out.println("Done.");

        // Create lock commands hash.
        System.out.print("Loading commands config ... ");
		String message = getCommand(config_commands);
        Map commands = new Gson().fromJson(message, Map.class);
        System.out.println("Done.");

        System.out.println("Initializing Sphinx ...");
        Recognizer recognizer = (Recognizer) cm.lookup("recognizer");
        recognizer.allocate();

        System.out.println("Starting Microphone ...");
        // start the microphone or exit if this is not possible
        Microphone microphone = (Microphone) cm.lookup("microphone");
        if (!microphone.startRecording()) {
            System.out.println("Cannot start microphone.");
            recognizer.deallocate();
            System.exit(1);
        }

        System.out.println("Start speaking. Press Ctrl-C to quit.\n");
        // Loop the recognition until the program exits.
        while (true) {

            Result result = recognizer.recognize();

            if (result != null) {

                ConfidenceScorer cs = (ConfidenceScorer) cm.lookup("confidenceScorer");
                ConfidenceResult cr = cs.score(result);
                Path best = cr.getBestHypothesis();

                // Print linear confidence of the best path
                System.out.println(best.getTranscription());
                System.out.println
                        ("     (confidence: " +
                                format.format(best.getLogMath().logToLinear
                                        ((float) best.getConfidence()))
                                + ')');
                System.out.println();

                String resultText = result.getBestFinalResultNoFiller().trim();

                // Look up resultText in lock_commands hash
                String cmd = (String)commands.get(resultText);

                System.out.println("[" + resultText + "]");

                //if (cmd != "") {
                if (commands.containsValue(cmd)) {
                    System.out.println("Sending Command: " + cmd + '\n');
                    t = new Thread(new CustomProcess(cmd, "."));
                    t.start();
                } else {
                    System.out.println("I can't hear what you said.\n");
                }
            }
        }
    }

	private static String getCommand(String cmdFile) 
    {
        String message = "";
        try {
            Scanner sc = new Scanner(new File(cmdFile));
            while ( sc.hasNext() )
                message += sc.nextLine();
        } catch(FileNotFoundException e) {
            System.err.println("[Client] ** Command file not found");
        }
        return message;
    }
 }


