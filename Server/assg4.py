from socket import *
import sys
import PIL.Image as Image
import io
import base64

 
serverSocket = socket(AF_INET,SOCK_STREAM)
port = 1234 
serverSocket.bind(('192.168.1.45', port))
serverSocket.listen(1)
buff_size=4096


while True: 
	print ('Ready to serve....' )
	connectionSocket,addr = serverSocket.accept()
	connectionSocket.settimeout(1.0)
	data = b''
	dataSize = 0 
	while True:
		try: 
			partData = connectionSocket.recv(1024)
			print("Recived Image Packet Size:", len(partData))
			data += partData		
		except timeout:
			print ("--------------------------------------------")
			print ("timeout: Total Data Retrived:", len(data))
			print ("Raw Image Data Below")
			print ("--------------------------------------------")
			dataen= base64.b64decode(data)
			print(data)
			img=Image.open(io.BytesIO(dataen))
			
			img.save("ImageFromClient.bmp")
			connectionSocket.close()
			break 	
	 
		
		