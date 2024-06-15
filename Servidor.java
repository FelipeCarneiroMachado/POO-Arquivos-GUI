import java.net.ServerSocket;
import java.net.Socket;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.EOFException;

public class Servidor {
    public static void main(String[] args) throws IOException {
        ServerSocket servidor = new ServerSocket(5555);
        System.out.println("Porta 5555 aberta!");

        Socket cliente = servidor.accept();
        System.out.println("Nova conexão com o cliente " + cliente.getInetAddress().getHostAddress());

        DataInputStream in = new DataInputStream(cliente.getInputStream());
        DataOutputStream out = new DataOutputStream(cliente.getOutputStream());
        boolean running = true;
        while (running) {
            try {
                if (in.available() > 0) {
                    // Lê os dados enviados (Char, tamanho, string)
                    char type = (char) in.readByte();
                    int length = Integer.reverseBytes(in.readInt());
                    byte[] data = new byte[length];
                    if (length >= 0) {
                        in.readFully(data);
                    }

                    System.out.println("Tipo: " + type);
                    switch (type) {
                        case 'I':
                            System.out.println("Tamanho: " + length);
                            System.out.println("Dados Jogador: " + new String(data));
                            break;
                        case 'S':
                            System.out.println("Salvando");
                            break;
                        case 'Q':
                            System.out.println("Tamanho: " + length);
                            System.out.println("Dados de busca: " + new String(data));
                            String response = "123456,19,ENDRIK,PALMEIRAS,BRASIL";

                            //Se for mais de uma, usa isso aqui tbm
                            //out.writeUTF(String.join("|", jogadores));

                            //Publicação de volta para o canal do cliente
                            out.writeUTF(response);  
                            break;
                        case 'L':
                            System.out.println("Tamanho: " + length);
                            System.out.println("Data path: " + new String(data));
                            break;
                        case 'R':
                            System.out.println("Tamanho: " + length);
                            System.out.println("ID do jogador a ser removido: " + new String(data));
                            break;
                        case 'A':
                            System.out.println("Acessando dados dos registros");
                            String[] jogadores = {"123459,24,KARLOS,BAHIA,RUSSIA", "123456,19,ENDRIK,PALEMIRAS,BRASIL"};

                            //Publicação de volta para o canal do cliente
                            out.writeUTF(String.join("|", jogadores));
                            break;
                        case 'E':
                            System.out.println("Fechando o servidor");
                            in.close();
                            out.close();
                            cliente.close();
                            servidor.close();
                            running = false;
                            break;
                    
                        default:
                            break;
                    }
                }
            } catch (EOFException e) {
                System.out.println("Fim do stream");
                break; // Encerra o loop se o fim do stream for alcançado
            }
        }

        // Garante que todos os recursos sejam fechados fora do loop
        in.close();
        out.close();
        cliente.close();
        servidor.close();
    }
}
