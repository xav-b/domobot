/*
 * HumanEmulator.java
 * Copyright (C) 2013 xavier <xavier@laptop-300E5A>
 *
 * Distributed under terms of the MIT license.
 */

//package voice.commands;

import java.awt.AWTException;
import java.awt.Robot;
import java.awt.event.InputEvent;
import java.awt.event.KeyEvent;

/**  
 * Programmatic interaction with user environment
 */
public class HumanEmulator
{
    Robot robot = new Robot();

    public static void main(String[] args) throws Exception {
        new HumanEmulator();
        System.exit(0);
    }

	public HumanEmulator() throws AWTException {
        robot.setAutoDelay(50);
        //robot.mouseWheel(100);
        robot.mouseMove(300, 300);
        leftClick();
        type(KeyEvent.VK_ESCAPE);
        type(KeyEvent.VK_COMMA);
        type(KeyEvent.VK_S);
        //robot.keyPress(KeyEvent.VK_COLON);
        //robot.keyRelease(KeyEvent.VK_COLON);
        //type("wqa");
        //type(KeyEvent.VK_W);
        //type(KeyEvent.VK_Q);
        //type(KeyEvent.VK_A);
        //robot.delay(500);
        //type(KeyEvent.VK_ENTER);

	}
    private void leftClick()
    {
        robot.mousePress(InputEvent.BUTTON1_MASK);
        robot.mouseRelease(InputEvent.BUTTON1_MASK);
    }

    private void rightClick()
    {
        robot.mousePress(InputEvent.BUTTON3_MASK);
        robot.mouseRelease(InputEvent.BUTTON3_MASK);
    }
  
    private void type(int i)
    {
        robot.keyPress(i);
        robot.keyRelease(i);
    }

    private void type(String s)
    {
        byte[] bytes = s.getBytes();
        for (byte b : bytes)
        {
            int code = b;
            // keycode only handles [A-Z] (which is ASCII decimal [65-90])
            if (code > 96 && code < 123) 
                code = code - 32;

            robot.keyPress(code);
            robot.keyRelease(code);
        }
    }
}

