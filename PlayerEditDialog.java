import javax.swing.*;
import java.awt.*;

public class PlayerEditDialog extends JDialog {
    private JTextField idField, ageField, nameField, clubField, nationalityField;
    private JButton saveButton, deleteButton;
    private PlayerEditListener listener;

    public PlayerEditDialog(String playerData, PlayerEditListener listener1) {
        this.listener = listener1;
        setLayout(new GridLayout(6, 2));
        setTitle("Editar Jogador");
        setSize(300, 200);

        String[] data = playerData.split(",");
        add(new JLabel("ID:"));
        idField = new JTextField(data[0]);
        idField.setEditable(false);  
        add(idField);

        add(new JLabel("Idade:"));
        ageField = new JTextField(data[1]);
        add(ageField);

        add(new JLabel("Nome:"));
        nameField = new JTextField(data[2]);
        add(nameField);

        add(new JLabel("Nacionalidade:"));
        nationalityField = new JTextField(data[4]);
        add(nationalityField);

        add(new JLabel("Clube:"));
        clubField = new JTextField(data[3]);
        add(clubField);

        saveButton = new JButton("Salvar");
        saveButton.addActionListener(e -> {
            if (listener != null) {
                String playerInfo = String.format("%s,%s,%s,%s,%s",
                        idField.getText(),  
                        ageField.getText(),
                        nameField.getText(),
                        nationalityField.getText(),
                        clubField.getText());
                listener.onPlayerEdit(playerInfo, 'U');  // Assume 'U' como update
            }
            dispose();
        });
        add(saveButton);

        deleteButton = new JButton("Excluir");
        deleteButton.addActionListener(e -> {
            if (listener != null) {
                listener.onPlayerEdit(idField.getText(), 'D');  // Passa apenas o ID para exclusão
            }
            dispose();
        });
        add(deleteButton);

        setVisible(true);
    }
}
