import javax.swing.*;
import java.awt.*;
import java.awt.event.*;

public class GUI extends JFrame {

    public GUI(){
        // Cria o Jframe com layout
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setSize(960,540);
        GridBagLayout layout = new GridBagLayout();
        setLayout(layout);

        JButton searchButton = new JButton("Search player");
        searchButton.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                    new SearchWindow();
            }
        });
        add(searchButton);

        JButton insertButton = new JButton("Insert player");
        insertButton.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                new InsertWindow();
            }
        });
        add(insertButton);

        setVisible(true);
    }

    public static void main(String[] args)
    {
        new GUI();
    }
}
