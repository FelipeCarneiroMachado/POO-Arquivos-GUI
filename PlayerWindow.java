import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import javax.swing.text.NumberFormatter;
import java.text.NumberFormat;
import java.text.ParseException;

public class PlayerWindow extends JFrame {
    private PlayerDataListener listener;

    public PlayerWindow(String action, String title, PlayerDataListener listener1) {
        this.listener = listener1;

        setTitle(title);
        setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);

        // Create components
        JLabel idLabel = new JLabel("ID:");
        JLabel ageLabel = new JLabel("Idade:");
        JLabel nameLabel = new JLabel("Nome:");
        JLabel nationalityLabel = new JLabel("Nacionalidade:");
        JLabel clubLabel = new JLabel("Nome do clube:");

        NumberFormat integerFormat = NumberFormat.getIntegerInstance();
        integerFormat.setGroupingUsed(false);
        NumberFormatter numberFormatter = new NumberFormatter(integerFormat);
        numberFormatter.setValueClass(Integer.class);
        numberFormatter.setAllowsInvalid(true); // Allow temporarily invalid values for editing flexibility
        numberFormatter.setMinimum(0); // Optional: Set minimum value as 0 if negative numbers are not valid

        JFormattedTextField idTextField = new JFormattedTextField(numberFormatter);
        idTextField.setColumns(20);
        JFormattedTextField ageTextField = new JFormattedTextField(numberFormatter);
        ageTextField.setColumns(20);

        JTextField nameTextField = new JTextField(20);
        JTextField nationalityTextField = new JTextField(20);
        JTextField clubTextField = new JTextField(20);

        JButton button = new JButton(action);
        button.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                try {
                    idTextField.commitEdit();
                    ageTextField.commitEdit();
                } catch (ParseException ex) {
                    JOptionPane.showMessageDialog(null, "ID e Idade devem ser números inteiros.", "Erro de Formatação", JOptionPane.ERROR_MESSAGE);
                    idTextField.setValue(null);
                    ageTextField.setValue(null);
                    return;
                }

                Object idValue = idTextField.getValue();
                Object ageValue = ageTextField.getValue();
                String id = (idValue != null) ? idValue.toString() : "N/A";
                String age = (ageValue != null) ? ageValue.toString() : "N/A";
                String name = nameTextField.getText().trim().toUpperCase();
                String nationality = nationalityTextField.getText().trim().toUpperCase();
                String club = clubTextField.getText().trim().toUpperCase();

                String playerInfo = String.format("%s,%s,%s,%s,%s", id, age, name, nationality, club);

                if (listener != null) {
                    listener.onDataReady(playerInfo);
                }

                dispose();
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
        add(nationalityLabel, gbc);
        gbc.gridx = 1;
        add(nationalityTextField, gbc);

        gbc.gridx = 0;
        gbc.gridy = 4;
        add(clubLabel, gbc);
        gbc.gridx = 1;
        add(clubTextField, gbc);

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
        new PlayerWindow("Salvar", "Cadastro de Jogador", new PlayerDataListener() {
            @Override
            public void onDataReady(String data) {
                System.out.println("Dados do Jogador: " + data);
            }
        });
    }
}
