const socket = io('http://10.65.13.3:5000');

function handleSubmit()
{
    console.log("handleSubmit");
    const text = document.getElementById('messageInput').value;
    socket.emit('message', `${text}`);
}

socket.on('Ack', text => {
 
    const el = document.createElement('li');
    el.innerHTML = text;
    document.querySelector('ul').appendChild(el);
});
 
