import javax.swing.*;
import java.awt.*;
import java.awt.event.*;

public class PlayersViewWindow extends JFrame {
    private JList<String> list;
    private DefaultListModel<String> listModel;
    private PlayerEditListener listener;  // Adicionar um campo para o listener

    public PlayersViewWindow(String[] jogadores, PlayerEditListener listener1) {
        this.listener = listener1;  // Inicializar o listener
        setTitle("Resultado da Busca");
        setSize(500, 700);
        setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
        setLayout(new BorderLayout());

        // Modelo da lista e JList para exibição dos jogadores
        listModel = new DefaultListModel<>();
        for (String jogador : jogadores) {
            listModel.addElement(jogador);
        }
        list = new JList<>(listModel);
        JScrollPane scrollPane = new JScrollPane(list);
        add(scrollPane, BorderLayout.CENTER);

        // Painel para botões
        JPanel buttonPanel = new JPanel();
        buttonPanel.setLayout(new FlowLayout(FlowLayout.CENTER));

        // Botão para editar o jogador selecionado
        JButton editButton = new JButton("Editar");
        editButton.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                String selectedPlayer = list.getSelectedValue();
                if (selectedPlayer != null) {
                    new PlayerEditDialog(selectedPlayer, listener);
                } else {
                    JOptionPane.showMessageDialog(null, "Selecione um jogador para editar.");
                }
            }
        });
        buttonPanel.add(editButton);

        // Botão para fechar a janela
        JButton closeButton = new JButton("Fechar");
        closeButton.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                dispose();  // Fecha esta janela
            }
        });
        buttonPanel.add(closeButton);

        // Adiciona o painel de botões ao sul do BorderLayout
        add(buttonPanel, BorderLayout.SOUTH);

        setVisible(true);
    }
}
