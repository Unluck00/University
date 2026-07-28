import java.io.*; 
import java.net.*; 

class TCPServer { 

  public static void main(String argv[]) throws Exception 
    { 
      String clientSentence; 
      String whatToDo; 
      String capitalizedSentence; 
    try {
      ServerSocket welcomeSocket = new ServerSocket(6789); 
  
      while(true) { 
  
            Socket connectionSocket = welcomeSocket.accept(); 

           BufferedReader inFromClient = 
              new BufferedReader(new
              InputStreamReader(connectionSocket.getInputStream())); 


           DataOutputStream  outToClient = 
             new DataOutputStream(connectionSocket.getOutputStream()); 

           whatToDo = inFromClient.readLine(); 
           clientSentence = inFromClient.readLine(); 

	   if (whatToDo.equals("upper"))
	           capitalizedSentence = clientSentence.toUpperCase() + '\n'; 
	   else    capitalizedSentence = clientSentence.toLowerCase() + '\n';
           
	   outToClient.writeBytes(capitalizedSentence); 
        } 
 
   } catch (Exception e) {}
    }
} 
 

