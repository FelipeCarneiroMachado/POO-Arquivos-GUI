import javax.swing.*;
import javax.swing.filechooser.FileNameExtensionFilter;

import java.awt.*;
import java.awt.event.*;
import java.io.IOException;


public class GUI extends JFrame {
    private PlayersViewWindow playersViewWindow;
    public String filename = null;
    String[] jogadores = null;

    private void editPlayer(String[] jogadores, String editedPlayer) {
        String id = editedPlayer.split(",")[0]; 
        System.out.println("Editando jogador com ID: " + id);
        for (int i = 0; i < jogadores.length; i++) {
            if (jogadores[i].startsWith(id + ",")) { 
                jogadores[i] = editedPlayer; // Atualiza o jogador.
                System.out.println("Jogador editado: " + jogadores[i]); //Debug. Tirar dps
                break; 
            }
        }
    }

    private void excludePlayer(String[] jogadores, String id) {
        System.out.println("Excluindo jogador com ID: " + id);
        for (int i = 0; i < jogadores.length; i++) {
            if (jogadores[i].startsWith(id + ",")) { 
                for (int j = i; j < jogadores.length - 1; j++) {
                    jogadores[j] = jogadores[j + 1];
                    System.out.println("Jogador excluído: " + jogadores[j]); //Debug. Tirar dps
                }
                jogadores[jogadores.length - 1] = null; 
                break; 
            }
        }
    }
    

    public GUI(){

        //Inicialização de um cliente por GUI
        try {
            Cliente client = new Cliente("127.0.0.1", 5550);
            System.out.println("Entrando com o cliente");
            GUI gui = this;
            filename = null;

            setTitle("Gerenciador de registros de jogadores do FIFA");
            setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
            setSize(960,540);
            GridBagLayout layout = new GridBagLayout();
            setLayout(layout);
            
            //Botão para carregar arquivo
            JButton fileSelectButton = new JButton("Carregar arquivo");
            fileSelectButton.addActionListener(new ActionListener() {
                public void actionPerformed(ActionEvent e) {
                    JFileChooser fileChooser = new JFileChooser();
                    FileNameExtensionFilter filter = new FileNameExtensionFilter("Binary files", "bin", "csv");
                    fileChooser.setFileFilter(filter);

                    if(fileChooser.showOpenDialog(gui) == JFileChooser.APPROVE_OPTION) {
                        //Aqui filename é definida como o nome do arquivo selecionado
                        filename = fileChooser.getSelectedFile().getName();

                        //Envia o caminho do arquivo para o servidor
                        try {
                            client.sendLoad(fileChooser.getSelectedFile().getAbsolutePath());
                        } catch (IOException ioException) {
                            ioException.printStackTrace();
                        }
                    }
                }
            });
            add(fileSelectButton);

            //Botão para salvar o arquivo
            JButton saveFileButton = new JButton("Salvar arquivo");
            saveFileButton.addActionListener(new ActionListener() {
                public void actionPerformed(ActionEvent e) {
                    if(filename != null){
                        //Envia instrução para salvar o arquivo passado anteriormente
                        try {
                            client.sendSave();
                        } catch (IOException ioException) {
                            ioException.printStackTrace();
                        }
                    }
                    else{
                        // Exibe uma mensagem de erro se nenhum arquivo foi carregado
                        JOptionPane.showMessageDialog(gui, "Nenhum arquivo carregado. Carregue um arquivo antes de tentar salvar.", "Erro", JOptionPane.ERROR_MESSAGE);
                    }
                }
            });

            add(saveFileButton);

            // Botão para listar jogadores
            JButton listPlayersButton = new JButton("Listar Jogadores");
            listPlayersButton.addActionListener(new ActionListener() {
                public void actionPerformed(ActionEvent e) {
                    if (filename != null) {
                        try {
                            client.sendAcessData(filename);
                            Thread.sleep(2000);
                            System.out.println("Ta aq");
                            //Recebe a resposta do servidor com TODOS os registros disponíveis
                            jogadores = client.receiveResponse();
                            
                            //String[] jogadores = {"123456,69,ANDRE,BAHIA,REMO", "123456,69,ANDRE,BAHIA,REMO"};
                            
                            if (playersViewWindow != null) {
                                playersViewWindow.dispose(); // Fecha a janela de visualização de jogadores se ela já estiver aberta (att os jogadores)
                            }
                            // Cria uma nova janela com os dados atualizados e a mostra
                            PlayerEditListener editListener = new PlayerEditListener() {
                                public void onPlayerEdit(String data, char operation) {
                                    try {
                                        if (operation == 'U') {
                                            client.sendInsert(data);
                                            editPlayer(jogadores, data);
                                        } else if (operation == 'D') {
                                            client.sendRemove(data);
                                            excludePlayer(jogadores, data);
                                        }
                                        if (playersViewWindow != null) {
                                            playersViewWindow.dispose();
                                        }
                                        playersViewWindow = new PlayersViewWindow(jogadores, this);
                                        playersViewWindow.setVisible(true);
                                    } catch (IOException e) {
                                        e.printStackTrace();
                                    }
                                }
                            };
                            playersViewWindow = new PlayersViewWindow(jogadores, editListener);
                            playersViewWindow.setVisible(true);
                        } catch (IOException ioException) {
                            ioException.printStackTrace();
                        }
                        catch (InterruptedException ie) {
                            Thread.currentThread().interrupt();  // Restaura o status de interrupção
                            ie.printStackTrace();
                        } 
                    } else {
                        JOptionPane.showMessageDialog(gui, "Nenhum arquivo carregado. Carregue um arquivo antes de tentar salvar.", "Erro", JOptionPane.ERROR_MESSAGE);
                    }
                }
            });
            add(listPlayersButton);

            //Botão para buscar jogador
            JButton searchButton = new JButton("Buscar jogador");
            searchButton.addActionListener(new ActionListener() {
                public void actionPerformed(ActionEvent e) {
                    if(filename != null){
                        PlayerDataListener listener = new PlayerDataListener() {
                            public void onDataReady(String playerData) {
                                System.out.println("Dados Recebidos: " + playerData);

                                try {
                                    client.sendQuery(playerData);
                                    Thread.sleep(500);

                                    //Recebe a resposta do servidor com os registros que correspondem à busca
                                    jogadores = client.receiveResponse(); 
                                } 
                                    catch (InterruptedException ie) {
                                    Thread.currentThread().interrupt();  // Restaura o status de interrupção
                                    ie.printStackTrace();
                                } catch (IOException ioException) {
                                    ioException.printStackTrace();
                                }

                                //String[] jogadores = {"123456,69,ANDRE,BAHIA,REMO", "123456,69,ANDRE,BAHIA,REMO"};
                                PlayerEditListener editListener = new PlayerEditListener() {
                                    public void onPlayerEdit(String data, char operation) {
                                        try {
                                            if (operation == 'U') {
                                                client.sendInsert(data);
                                                editPlayer(jogadores, data);
                                            } else if (operation == 'D') {
                                                client.sendRemove(data);
                                                excludePlayer(jogadores, data);
                                            }
                                            if (playersViewWindow != null) {
                                                playersViewWindow.dispose();
                                            }
                                            playersViewWindow = new PlayersViewWindow(jogadores, this);
                                            playersViewWindow.setVisible(true);
                                        } catch (IOException e) {
                                            e.printStackTrace();
                                        }
                                    }
                                };
                                playersViewWindow = new PlayersViewWindow(jogadores, editListener);
                                playersViewWindow.setVisible(true);
                            }
                        };
                        new SearchWindow(listener);
                    }
                    else{
                        // Exibe uma mensagem de erro se nenhum arquivo foi carregado
                        JOptionPane.showMessageDialog(gui, "Nenhum arquivo carregado. Carregue um arquivo antes de tentar salvar.", "Erro", JOptionPane.ERROR_MESSAGE);
                    }
                }
            });
            add(searchButton);

            // Botão para fechar a GUI
            JButton closeButton = new JButton("Fechar Interface");
            closeButton.addActionListener(new ActionListener() {
                public void actionPerformed(ActionEvent e) {
                    // Envia um comando para fechar o servidor
                    try {
                        client.sendExit();
                        client.close();
                    } catch (IOException ioException) {
                        ioException.printStackTrace();
                    }

                    // Fecha a janela da GUI
                    dispose();
                }
            });
            add(closeButton);

            

            setVisible(true);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public static void main(String[] args)
    {
        new GUI();
    }
}
