import java.io.*;
import java.net.Socket;
import java.nio.charset.StandardCharsets;
import java.util.Scanner;

public class Cliente {
    private Socket socket;
    private DataOutputStream out;
    private DataInputStream in;

    public Cliente(String address, int port) throws IOException {
        socket = new Socket(address, port);
        out = new DataOutputStream(socket.getOutputStream());
        in = new DataInputStream(socket.getInputStream());
    }

    //Outra oçõa aqui seria usar String
    private void sendInstruction(char type, byte[] data) throws IOException {
        System.out.println(data.length);
        out.writeByte(type); // Escreve o tipo de instrução
        out.writeInt(data.length); // Escreve o comprimento em little endian
        out.write(data); // Escreve os dados
        out.flush();
    }

    // public String[] receiveResponse() throws IOException {
    //     if (in != null && in.available() > 0) {  // Checa se 'in' não é nulo e se há dados disponíveis
    //         System.out.println("Travou na soket");
    //         String response = in.readUTF();      // Lê a resposta do servidor
    //         System.out.println("Resposta do servidor: " + response);
    //         return response.split("\\|");        // Retorna os dados separados por '|', sem contar espaços extras
    //     } else {
    //         System.out.println("Nenhum dado disponível para leitura ou stream é nulo.");
    //     }
    //     // Casos de erro
    //     return new String[0];
    // }
    public String[] receiveResponse() throws IOException {
        if (in != null && in.available() > 0) {  // Checa se 'in' não é nulo e se há dados disponíveis
            System.out.println(in.readByte());
            int length = in.readInt();           // Lê o comprimento da mensagem esperada
            if (length > 0) {
                byte[] data = new byte[length];
                in.readFully(data);              // Lê os dados em bytes do servidor
                String response = new String(data, StandardCharsets.UTF_8);  // Converte os bytes para string usando UTF-8
                System.out.println("Resposta do servidor: " + response);
                return response.split("\\|");    // Retorna os dados separados por '|', sem contar espaços extras
            } else {
                System.out.println("Resposta do servidor: comprimento de dados é 0.");
            }
        } else {
            System.out.println("Nenhum dado disponível para leitura ou stream é nulo.");
        }
        // Casos de erro
        return new String[0];
    }
    
    public void sendSave() throws IOException {
        sendInstruction('S', new byte[0]); // Sem dados para salvar
    }

    //A inserção é realizada quando há o update dos dados de um jogador
    public void sendInsert(String playerData) throws IOException {
        sendInstruction('I', playerData.getBytes(StandardCharsets.UTF_8));
    }

    //A remoção é sempre feita pelo id do registro
    public void sendRemove(String id) throws IOException {
        sendInstruction('R', id.getBytes(StandardCharsets.UTF_8));
    }
    
    public void sendLoad(String filePath) throws IOException {
        sendInstruction('L', filePath.getBytes(StandardCharsets.UTF_8));
    }

    public void sendQuery(String query) throws IOException {
        sendInstruction('Q', query.getBytes(StandardCharsets.UTF_8));
    }

    //Publicação para acessar todos ols registros de dados disponíveis no arquivo carregado
    public void sendAcessData(String data) throws IOException {
        sendInstruction('A', new byte[0]);
    }

    public void sendExit() throws IOException {
        sendInstruction('E', new byte[0]); 

        try {
            out.close();
            //in.close(); Ainda tenho que definir o canal de in
            socket.close();
        } catch (IOException e) {
            System.out.println("Erro ao fechar recursos do cliente: " + e.getMessage());
        }
    }

    public void close() throws IOException {
        out.close();
        socket.close();
    }


    //Teste do servidor
    public static void main(String[] args) {
        try {
            Cliente client = new Cliente("127.0.0.1", 5050);
            Scanner s = new Scanner(System.in);
            while (true) {
                String text = s.nextLine();
                try {
                    if(text.equals("save"))
                        client.sendSave();
                    else if(text.equals("insert"))
                        client.sendInsert("Ola, meu amigo!");
                    else if(text.equals("load"))
                        client.sendLoad("C:\\Users\\gabri\\Desktop\\teste.bin");
                    else if(text.equals("query"))
                        client.sendQuery("SELECT * FROM players WHERE age > 30");
                    else if(text.equals("exit")) {
                        client.sendExit();
                        client.close();
                        s.close();  
                        break;
                    }
                
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
       
    }
}
