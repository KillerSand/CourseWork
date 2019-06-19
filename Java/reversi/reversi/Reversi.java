import javax.swing.*;

import java.awt.*;
import java.awt.event.*;
import java.util.ArrayList;

public class Reversi extends JPanel {
	private boolean blackTurn = true;
	protected ArrayList<RoundedButton> list;
	protected int blackTemp = 2, whiteTemp = 2;
	private boolean eightByEight = true;
	public Reversi()
	{
		list = new ArrayList<RoundedButton>();
		createContents();
		setVisible(true);
	}
	public Reversi(boolean eightByEight){
		list = new ArrayList<RoundedButton>();
		this.eightByEight = eightByEight;
		createContents();
		setVisible(true);
	}
	public boolean isEightByEight() {
		return eightByEight;
	}
	public void setEightByEight(boolean eightByEight) {
		this.eightByEight = eightByEight;
	}
	public void setBlackTurn(boolean blackTurn) {
		this.blackTurn = blackTurn;
	}
	public boolean getBlackTurn(){
		return this.blackTurn;
	}
	
	private void createContents(){
		RoundedButton button; // re-instantiate this button and use
		// to fill entire board
		if(eightByEight == true){
			setLayout(new GridLayout(8, 8));
		 for (int i=0; i<8; i++)
		    {
		      for (int j=0; j<8; j++)
		      {
		    	button = new RoundedButton();
		    	((RoundedButton) button).setGray();
		    	list.add((RoundedButton) button);
		        button.addActionListener(new Listener());
		        add(button);
		      } // end for j
		    } // end for i
		list.get(27).setWhite();
		list.get(28).setBlack();
		list.get(35).setBlack();
		list.get(36).setWhite();
		}
		else{
			setLayout(new GridLayout(6, 6));
			 for (int i=0; i < 6; i++)
			    {
			      for (int j=0; j < 6; j++)
			      {
			    	button = new RoundedButton();
			    	((RoundedButton) button).setGray();
			    	list.add((RoundedButton) button);
			        button.addActionListener(new Listener());
			        add(button);
			      } // end for j
			    } // end for i
			list.get(14).setWhite();
			list.get(15).setBlack();
			list.get(20).setBlack();
			list.get(21).setWhite();
		}
	} // end createContents
	 private class Listener implements ActionListener
	  {
	    public void actionPerformed(ActionEvent e)
	    {
	    	RoundedButton btn = new RoundedButton();
	    	btn = (RoundedButton) e.getSource();
	        	if(blackTurn){
	        		if(btn.isWhite()){
	        		btn.setBlack();
	        		whiteTemp--;
	        		blackTemp++;
	        		}
	        		else if(btn.isGray()){
	        			btn.setBlack();
	        			blackTemp++;
	        		}
	        	}
	        	
	        	else{
	        		if(btn.isBlack()){
	        			btn.setWhite();
	        			blackTemp--;
	        			whiteTemp++;
	        			
	        		}
	        		else if(btn.isGray()){
	        		btn.setWhite();
	        		whiteTemp++;
	        		}
	        	}
	    } // end actionPerformed
	  } // end class Listener

}
