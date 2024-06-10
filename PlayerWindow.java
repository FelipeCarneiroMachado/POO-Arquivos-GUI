import javax.swing.*;
import java.awt.*;
import java.awt.event.*;

public class PlayerWindow extends JFrame {

    public PlayerWindow(String action, String title) {

        setTitle(title);
        setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);

        // Create components
        JLabel idLabel = new JLabel("ID:");
        JLabel ageLabel = new JLabel("Idade:");
        JLabel nameLabel = new JLabel("Nome:");
        JLabel clubLabel = new JLabel("Nome do clube:");
        JLabel nationalityLabel = new JLabel("Nacionalidade:");

        JTextField idTextField = new JTextField(20);
        JTextField ageTextField = new JTextField(20);
        JTextField nameTextField = new JTextField(20);
        JTextField clubTextField = new JTextField(20);
        JTextField nationalityTextField = new JTextField(20);

        JButton button = new JButton(action);
        button.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                // Implement search functionality here
                // For now, just display a message
                JOptionPane.showMessageDialog(null, "Not yet implemented");
            }
        });

        // Set layout manager
        setLayout(new GridBagLayout());
        GridBagConstraints gbc = new GridBagConstraints();
        gbc.insets = new Insets(5, 5, 5, 5); // Padding

        // Add components to the frame
        gbc.gridx = 0;
        gbc.gridy = 0;
        add(idLabel, gbc);
        gbc.gridx = 1;
        add(idTextField, gbc);

        gbc.gridx = 0;
        gbc.gridy = 1;
        add(ageLabel, gbc);
        gbc.gridx = 1;
        add(ageTextField, gbc);

        gbc.gridx = 0;
        gbc.gridy = 2;
        add(nameLabel, gbc);
        gbc.gridx = 1;
        add(nameTextField, gbc);

        gbc.gridx = 0;
        gbc.gridy = 3;
        add(clubLabel, gbc);
        gbc.gridx = 1;
        add(clubTextField, gbc);

        gbc.gridx = 0;
        gbc.gridy = 4;
        add(nationalityLabel, gbc);
        gbc.gridx = 1;
        add(nationalityTextField, gbc);

        gbc.gridx = 0;
        gbc.gridy = 5;
        gbc.gridwidth = 2;
        gbc.anchor = GridBagConstraints.CENTER;
        add(button, gbc);

        // Pack and display the frame
        pack();
        setLocationRelativeTo(null);
        setVisible(true);
    }

    public static void main(String[] args) {
        SwingUtilities.invokeLater(new Runnable() {
            public void run() {
                new PlayerWindow("Example", "Example");
            }
        });
    }
}
