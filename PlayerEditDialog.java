import javax.swing.*;
import javax.swing.text.AbstractDocument;
import javax.swing.text.AttributeSet;
import javax.swing.text.BadLocationException;
import javax.swing.text.DocumentFilter;
import java.awt.*;

public class PlayerEditDialog extends JDialog {
    private JTextField idField, ageField, nameField, nationalityField, clubField;
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
        ((AbstractDocument) ageField.getDocument()).setDocumentFilter(new NumericDocumentFilter());
        add(ageField);

        add(new JLabel("Nome:"));
        nameField = new JTextField(data[2]);
        applyTextFilter(nameField);
        add(nameField);

        add(new JLabel("Nacionalidade:"));
        nationalityField = new JTextField(data[3]);
        applyTextFilter(nationalityField);
        add(nationalityField);

        add(new JLabel("Clube:"));
        clubField = new JTextField(data[4]);
        applyTextFilter(clubField);
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

    private void applyTextFilter(JTextField textField) {
        ((AbstractDocument) textField.getDocument()).setDocumentFilter(new DocumentFilter() {
            @Override
            public void replace(FilterBypass fb, int offset, int length, String text, AttributeSet attrs) throws BadLocationException {
                text = text.replaceAll("[^\\p{ASCII}]", ""); // Remove non-ASCII characters, including accents
                super.replace(fb, offset, length, text, attrs);
            }
        });
    }

    private class NumericDocumentFilter extends DocumentFilter {
        @Override
        public void insertString(FilterBypass fb, int offset, String string, AttributeSet attr) throws BadLocationException {
            if (string.matches("\\d+")) {
                super.insertString(fb, offset, string, attr);
            }
        }

        @Override
        public void replace(FilterBypass fb, int offset, int length, String text, AttributeSet attrs) throws BadLocationException {
            if (text.matches("\\d+")) {
                super.replace(fb, offset, length, text, attrs);
            }
        }
    }
}
