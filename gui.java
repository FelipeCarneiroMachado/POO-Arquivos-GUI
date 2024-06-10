import javax.swing.*;
import javax.swing.filechooser.FileNameExtensionFilter;

import java.awt.*;
import java.awt.event.*;

public class GUI extends JFrame {

    public String filename;

    public GUI(){

        GUI gui = this;
        filename = null;

        setTitle("Gerenciador de registros de jogadores do FIFA");
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setSize(960,540);
        GridBagLayout layout = new GridBagLayout();
        setLayout(layout);

        JButton fileSelectButton = new JButton("Carregar arquivo");
        fileSelectButton.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                JFileChooser fileChooser = new JFileChooser();
                FileNameExtensionFilter filter = new FileNameExtensionFilter("Binary files", "bin");
                fileChooser.setFileFilter(filter);

                if(fileChooser.showOpenDialog(gui) == JFileChooser.APPROVE_OPTION) {
                    //Aqui filename é definida como o nome do arquivo selecionado
                    filename = fileChooser.getSelectedFile().getName();
                }
            }
        });
        add(fileSelectButton);

        JButton saveFileButton = new JButton("Salvar arquivo");
        saveFileButton.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                if(filename != null){
                    //Aqui implementa a aplicação das alterações no arquivo
                }
            }
        });
        add(saveFileButton);

        JButton searchButton = new JButton("Buscar jogador");
        searchButton.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                    new SearchWindow();
            }
        });
        add(searchButton);

        JButton insertButton = new JButton("Inserir jogador");
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
