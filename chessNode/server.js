var express=require('express');
var app=express();
var http=require('http');
var server=http.createServer(app);
const path =require('path');
const {Server}=require("socket.io");
const io=new Server(server, {
    cors: {
      //origin: "http://localhost:3000",
      origin: "*",
      methods: ["GET", "POST"]
    }
  });

//////////////////////////////////////////////////////////////////////////

//Firstly this
app.get("/",(req,res)=>{
    return res.sendFile(path.resolve(__dirname,"../Client/front.html"))
})
//Secondly this(required for serving all the static file required .png,.css ,etc.)
app.use(express.static(path.resolve(__dirname,"../Client")));

//(opposite order causes to server index.html by default)


// Socket.IO connection handler
io.on('connection', (socket) => {
    console.log('A user connected');

    // Handle chat messages
    socket.on('clientEmit', (msg) => {
        console.log('client message in server: ' + msg);
    });

    socket.emit("serverEmit","from server feedback");

    // Handle disconnection
    socket.on('disconnect', () => {
        console.log('User disconnected');
    });
});

server.listen(3000,()=>console.log('Server started at 3000'));

////////////////////////////////////////////////////////////////////////////////
