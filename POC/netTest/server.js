const express = require("express");
const cors = require("cors");
const http = require("http");
const net = require('net');
const  path = require('path');
const { Server } = require("socket.io");
const { clearInterval } = require("timers");


require("dotenv").config();


const app = express();
app.use(
  cors({
    origin: "*",
  })
);

app.use('/', express.static(path.join(__dirname, '/public')));


const server = http.createServer(app);
//create socket server
const io = new Server(server, {
  cors: {
    origin: "*",
  },
});

//Create a TCP client to connect to the C++ server
const cplusplusClient = new net.Socket();
cplusplusClient.connect(3000, '10.65.13.3');
cplusplusClient.setEncoding('utf-8');


cplusplusClient.on('connect',() => {
  console.log(`C++ connected on PORT 3000`);
});
cplusplusClient.on('close', () => {
  console.log("C++ Connection closed");
  cplusplusClient.connect(3000, '10.65.13.3');

})

var cppserver = require('socket.io-client')('http://10.65.13.3:3000');

//after connection with the node client
io.on("connection", (socket) => {
  
  socket.on('data' , data=>{
    buffer += data.toString();
    console.log('data is:' , buffer);
  });
  socket.on("message",async (msg) => {
    try{
      console.log(msg);
      cplusplusClient.write(msg);

      const intervalID = setInterval(() => {
        const response = cplusplusClient.read();
        if(response != null)
        {
          clearInterval(intervalID);
        }
        console.log("Response : ",response);
        socket.emit("Ack", response);
      },1000);
    }
    catch(error)
    {
      console.log("error : ", error);
    }
    
  });
});


//Server will listens for requests on port 5000
server.listen(5000, "10.65.13.3", () => {
  console.log(`Server is Running on PORT 5000`);
});
