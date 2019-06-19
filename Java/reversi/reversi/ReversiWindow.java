/*
 * Kirk Edwards
 */
import javax.swing.*;
import java.awt.*;
import java.awt.event.*;

public class ReversiWindow extends JFrame {
	Reversi reversi = new Reversi();
	int blackWins = 0, whiteWins = 0, tie = 0;
	JPanel buttons = new JPanel(new GridLayout(7,2));
	JButton doneMoving, newGame, endGame;
	JTextField blackPieces, whitePieces,turn, blackWin,whiteWin,ties;
	JRadioButton eightByEight, sixBySix,blackFirst,whiteFirst;
	boolean grid;

	public ReversiWindow(){
		setTitle("Reversi");
		setLayout(new BorderLayout());
		setSize(600, 600);
		setDefaultCloseOperation(EXIT_ON_CLOSE);
		createContents();
		setVisible(true);
	}
	private void createContents(){
		Listener listener = new Listener();
		doneMoving = new JButton("Done Moving");
		newGame = new JButton("New Game");
		endGame = new JButton("End Game");
		turn = new JTextField("Black's Turn");
		blackPieces = new JTextField("# of Black Pieces: 2");
		whitePieces = new JTextField("# of White Pieces: 2");
		sixBySix = new JRadioButton("6 x 6");
		eightByEight = new JRadioButton("8 x 8",true);
		blackFirst = new JRadioButton("Black First", true);
		whiteFirst = new JRadioButton("White First");
		blackWin = new JTextField("# of Black Wins: " + blackWins);
		whiteWin = new JTextField("# of White Wins: " + whiteWins);
		ties = new JTextField("# of Ties: " + tie);
		ButtonGroup size = new ButtonGroup();
		ButtonGroup turns = new ButtonGroup();
		size.add(sixBySix);
		size.add(eightByEight);
		turns.add(blackFirst);
		turns.add(whiteFirst);
		newGame.addActionListener(listener);
		endGame.addActionListener(listener);
		doneMoving.addActionListener(listener);
		buttons.add(blackPieces);
		buttons.add(whitePieces);
		buttons.add(blackFirst);
		buttons.add(whiteFirst);
		buttons.add(blackWin);
		buttons.add(whiteWin);
		buttons.add(sixBySix);
		buttons.add(eightByEight);
		buttons.add(newGame);
		buttons.add(endGame);
		buttons.add(doneMoving);
		buttons.add(ties);
		buttons.add(turn);
		add(reversi,BorderLayout.CENTER);
		add(buttons,BorderLayout.EAST);
	}
	private class Listener implements ActionListener{

		public void actionPerformed(ActionEvent e) {
			blackPieces.setText("# of Black Pieces: " + reversi.blackTemp);
			whitePieces.setText("# of White Pieces: " + reversi.whiteTemp);
			if(e.getSource() == newGame){
				remove(reversi);
				reversi = new Reversi(eightByEight.isSelected() ? true : false);
				add(reversi,BorderLayout.CENTER);
				turn.setText(blackFirst.isSelected() ? "Black's Turn" : "White's Turn");
				reversi.setBlackTurn(blackFirst.isSelected() ? true : false);
				endGame.setEnabled(true);
				validate();
				
			}
			else if(e.getSource() == doneMoving){
				reversi.setBlackTurn(!reversi.getBlackTurn());
				turn.setText(reversi.getBlackTurn() ? "Black's Turn" : "White's Turn");
			}
			else if(e.getSource() == endGame){
				if(reversi.blackTemp > reversi.whiteTemp){
					blackWins++;
					blackWin.setText("# of Black Wins: " + blackWins);
				}
				else if(reversi.blackTemp < reversi.whiteTemp){
					whiteWins++;
					whiteWin.setText("# of White Wins: " + whiteWins);
				}
				else{
					tie++;
					ties.setText("# of Ties: " + tie);
				}	
				reversi.setVisible(false);
				endGame.setEnabled(false);
			}			
		}
	}	
		
	
	
	public static void main(String[] args) {
		new ReversiWindow();
	}

}
