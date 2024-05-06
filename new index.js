////////////////////////////////////////////Main logic from here//////////////////////////////////////////////////////////////

//util import not working???
//changed index.js type to module also in index,html
//import {ID,strPosition,objPosition} from '../utils.js';

//ENUMS
const ID = Object.freeze({
  PLAYER1: "pl1",
  PLAYER2: "pl2"
});
//convert position in objectform(server) into stringform(client)
function strPosition(positionObj)
{
  return ""+positionObj.x+positionObj.y;
}
//convert position in objectform(server) into stringform(client)
function objPosition(positionStr)
{
  return { x: eval(positionStr[0]) , y: eval(positionStr[1])};
}

////////////////////////////////////////////////////////////////////////////////
const socket = io();
let currentTurn = ID.PLAYER1;

buildChessBoard();

//color board for current(default) theme
const themeSelect = document.getElementById('theme-select-board').value;
coloring(themeSelect,getPlayerId());

//Coloring the board on theme change
document.addEventListener('DOMContentLoaded', function() {
  const themeSelect = document.getElementById('theme-select-board');

  // Event listener for theme change
  themeSelect.addEventListener('change', function() {
    const selectedTheme = themeSelect.value;
    coloring(selectedTheme,getPlayerId());
  });

});

//Coloring the chat messages
document.addEventListener('DOMContentLoaded', function() {
  const themeSelectChat = document.getElementById('theme-select-board');
 
  // Event listener for theme change
  themeSelectChat.addEventListener('change', function() {
    const selectedTheme = themeSelectChat.value;
    coloringChat(selectedTheme);
  });
 
});
 
const themeSelectChat = document.getElementById('theme-select-board').value;
coloringChat(themeSelectChat);

socket.on("connect", () => {
  console.log("indexjs connected cid:", socket.id);  

  if(getPlayerId()== ID.PLAYER1)
  {
    socket.emit('createRoom',sessionStorage.getItem("createRoomId"));
  }
  else
  {
    socket.emit('joinRoom',sessionStorage.getItem("joinRoomId"));
  }
});

socket.on("startGame",()=>{
  //window.alert("start game signal");
  console.log("start game signal");
  //after game started apply drag and click to chess pieces
  applyDragEvent();
  applyClickEvent();
})

const chatTextInput = document.querySelector(".text_input");
chatTextInput.addEventListener("keypress",(e)=>{
  if(e.key==="Enter")
  {
    socket.emit("chatText",getPlayerId(),chatTextInput.value);
    chatTextInput.value="";//reset the text box
  }
})

socket.on("serverChatText",(playerId,msg)=>{
  if(playerId==getPlayerId())
  {
    console.log("Me:",msg);
    addMessage(msg,playerId);
  }
  else
  {  
    console.log("Opponent:",msg);
    addMessage(msg,playerId);
    
  }
  const selectedTheme=document.getElementById('theme-select-board').value;
  coloringChat(selectedTheme);
})
socket.on("serverPieceMove",(playerId,pieceId,oldPosition,newPosition)=>{

  oldPosition = strPosition(oldPosition);
  newPosition = strPosition(newPosition);

  let pieceToMove=document.getElementById(oldPosition).firstChild;
  //console.log("piece to move:",pieceToMove);
  let newSquare=document.getElementById(newPosition);

  let pieceKilled=false;
  //added to remove the piece if is killed
  if(newSquare.children.length!=0)
  {
    newSquare.removeChild(newSquare.firstChild);//kill(remove) piece
    pieceKilled=true;
  }

  newSquare.appendChild(pieceToMove);

  //print the move to the screen
  printMove(playerId,pieceId,newPosition,pieceKilled);

})
socket.on("changeTurn",(currentTurnServer)=>{
  currentTurn = currentTurnServer;
  console.log("change turn in client,currentTurn:",currentTurn);
});

socket.on("serverPieceFocus",(playerId,positionArray)=>{
  console.log("got array of pos");
  if(getPlayerId()==playerId)
  {
    let validMovesArr = [];
    for(let pos of positionArray)
    {
        let strPos=strPosition(pos);
        validMovesArr.push(strPos);
    }
    console.log("array recv",validMovesArr);
    highlightCells(validMovesArr);
  }
})

/////////////////Main Logic Ends/////////////////////////////////////////////////////////////////////////////

function getPlayerId()
{
    return sessionStorage.getItem("playerID");
}

function buildChessBoard()
{
  let playerId=getPlayerId();

  if(playerId== ID.PLAYER1)
  {
    //if white
    for(let row=8;row>=1;row--)
    {
      buildLogic(row,playerId);
    }
  }
  else
  { 
    //if black
    for(let row=1;row<=8;row++)
    {
      buildLogic(row,playerId);
    }
  }

  insertChessPiece();

  function buildLogic(row,playerId)
  {
    //create a div
    let rowDiv = document.createElement('div');
    rowDiv.className='div';
    rowDiv.id = `row${row}`;
    for(let i=1;i<=8;i++)
    {
      //ceate a newList ele
      let boxList=document.createElement('li');
      boxList.className = 'box';
      let colNum=(playerId==ID.PLAYER1? i:9-i);//if black column would be opposite 8,7...2,1
      boxList.id = `${row}${colNum}`;
      rowDiv.appendChild(boxList);
    }
    let boardUl=document.querySelector('ul');
    boardUl.appendChild(rowDiv);
  }

  //inserting the images
  function insertChessPiece() {
    document.querySelectorAll(".box").forEach((square) => {
      
      square.style.cursor = "pointer";

      let rowNum=eval(square.id[0]);

      if(rowNum==1 ||rowNum==2 ||rowNum==7||rowNum==8)
      {
        let imageName="";
        let pieceId="";
        
        if(rowNum == 2 )
        {  
          imageName="Wpawn";
          pieceId="pawn"+square.id[1];
        }
        else if( rowNum == 7)
        {
          imageName="Bpawn";
          pieceId="pawn"+square.id[1];//black and white same pieceid??
        }

        else if(square.id =="11" || square.id =="18")
        {  
          imageName="Wrook";
          pieceId="rook"+(square.id[1]=='1'? "1":"2");
        }
        else if(square.id =="12" || square.id =="17")
        {  
          imageName="Wknight";
          pieceId="knight"+(square.id[1]=='2'? "1":"2");
        }
        else if(square.id =="13" || square.id =="16")
        {
          imageName="Wbishop";
          pieceId="bishop"+(square.id[1]=='3'? "1":"2");
        }
        

        else if(square.id =="81" || square.id =="88")
        {  
          imageName="Brook";
          pieceId="rook"+(square.id[1]=='1'? "1":"2");
        }
        else if(square.id =="82" || square.id =="87")
        {  
          imageName="Bknight";
          pieceId="knight"+(square.id[1]=='2'? "1":"2");
        }
        else if(square.id =="83" || square.id =="86")
        {  
          imageName="Bbishop";
          pieceId="bishop"+(square.id[1]=='3'? "1":"2");
        }

        else if(square.id == "14")
        {
          imageName="Wqueen";
          pieceId="queen"
        }
        else if(square.id == "15")
        {
          imageName="Wking"
          pieceId="king"
        }
        else if(square.id == "84")
        {
          imageName="Bqueen"
          pieceId="queen"
        }
        else if(square.id == "85")
        {
          imageName="Bking"
          pieceId="king"
        }
        
        //dont know the meaning of class here
        square.innerHTML = `<img class='all-img all-pown' src='images/${imageName}.png' pieceid=${pieceId} draggable='true' alt=''>`;
        
      }
    });
  }


}

// Function to apply selected theme
function coloring(theme) {
  const boxes = document.querySelectorAll('.box');
  boxes.forEach((box, index) => {
    const getId = box.id;
    const arr = Array.from(getId);
    const aside = eval(arr.pop());
    const aup = eval(arr.shift());
    const a = aside + aup;

        if ((a) % 2 == 0) 
        {
          box.classList.remove('default-mode1', 'dark-mode1', 'light-mode1', 'red-mode1', 'blue-mode1', 'green-mode1','purple-mode1','magenta-mode1','orange-mode1');
          box.classList.add(theme +'-mode1');
        } 
        else 
        {
          box.classList.remove('dark-mode', 'light-mode', 'red-mode', 'blue-mode', 'green-mode','purple-mode','magenta-mode','orange-mode');
          box.classList.add(theme + '-mode');
        }
  });
}
// Function to apply selected theme
function coloringChat(theme) {
  const messages = document.querySelectorAll('.message');
  messages.forEach((message, index) => {
    const isLeftMessage = message.classList.contains('left');
    if (isLeftMessage) {
      message.classList.remove('default-mode1', 'dark-mode1', 'light-mode1', 'red-mode1', 'blue-mode1', 'green-mode1', 'purple-mode1', 'yellow-mode1', 'magenta-mode1', 'orange-mode1');
      message.classList.add(theme + '-mode1');
    } else {
      message.classList.remove('dark-mode', 'light-mode', 'red-mode', 'blue-mode', 'green-mode', 'purple-mode', 'yellow-mode', 'magenta-mode', 'orange-mode');
      message.classList.add(theme + '-mode');
    }
  });
}

function highlightCells(validMovesArr)
{
  validMovesArr.forEach(pos =>{
    const square = document.getElementById(pos);
    let color= (getPlayerId() == ID.PLAYER1 ? "W":"B");
    //if opponent - color red; else yellow
    if(square.children.length!=0 &&  square.firstChild.getAttribute('src')[7]!=color)
      square.style.backgroundColor = 'red';
    else
      square.style.backgroundColor = 'yellow';
  });
}
function unHighlightCells()
{
  const squares = document.querySelectorAll('.box');
  squares.forEach(square => {
      square.style.backgroundColor = '';
  });
}

//Function for applying chesspiece drag events
function applyDragEvent() {
  const pieces = document.querySelectorAll('.box img');//maybe can also select all by giving class name  to image and queryselector
  let draggedPiece = null;
  

  // Attach drag events to all only our chess pieces
  pieces.forEach(piece => {
      //if the piece image src is (same color 0:white & 1:black) then only add to the drag and click events
      let color= (getPlayerId() == ID.PLAYER1 ? "W":"B");
      if(piece.getAttribute('src')[7]==color)
      {
        piece.addEventListener('dragstart', dragStart);
        piece.addEventListener('dragend', dragEnd); 
      }
          
  });

  // Attach drop event to all chessboard squares on drag drop
  const squares = document.querySelectorAll('.box');
  squares.forEach(square => {
      square.addEventListener('dragover', dragOver);
      square.addEventListener('drop', drop);
  });

  function dragStart(e) {
    draggedPiece = this;
    draggedPiece.style.border = '2px solid red';
    e.dataTransfer.setData('text/plain', ''); // Required for Firefox
    ////////////////////////////////////////////////////////
    let pieceId= draggedPiece.getAttribute('pieceid');
    socket.emit("pieceFocus",getPlayerId(),pieceId);
    console.log("called on drag piece focus");
}

  function dragEnd(e) {
      draggedPiece.style.border = '';
      draggedPiece = null;
      unHighlightCells();   
  }

  function dragOver(e) {
      e.preventDefault();
  }

  function drop() {
      if (draggedPiece) {
        
          let oldPosition = draggedPiece.parentNode.id;
          let newPosition = this.id;
          let pieceId= draggedPiece.getAttribute('pieceid');

          console.log(getPlayerId(),pieceId,oldPosition,newPosition);
          socket.emit("pieceMove",getPlayerId(),pieceId, objPosition(oldPosition),objPosition(newPosition) );
        
      }
  }
}

//Function for applying chesspiece click events
function applyClickEvent() {
  const pieces = document.querySelectorAll('.box img');
  let clickedPiece=null;
  pieces.forEach(piece => {
    let color= (getPlayerId() == ID.PLAYER1? "W":"B");
    if(piece.getAttribute('src')[7]==color)
    {
      piece.addEventListener('click', function(event) {

        if (this.style.border === '2px solid red') {
          // If already highlighted, remove the highlight(go back to default)
          this.style.border = '';
          clickedPiece = null;
          console.log("removing highlight");
        }
        else
        {
          if(clickedPiece!=null)
          {
            console.log("cp not null");
            //if already a piece is clicked and we choose another piece,then save & highlight newer (and remove older)
            clickedPiece.style = '';
            //assign curr as clicked piece
            this.style.border = '2px solid red';
            clickedPiece = this;

            unHighlightCells();
          } 
          else
          {
            console.log("cp is null");
            // If not highlighted, apply the highlight and save this piece
            this.style.border = '2px solid red';
            clickedPiece = this;
          }
          console.log("outside if else");
          //highlight poss. moves
          let pieceId= clickedPiece.getAttribute('pieceid');
          socket.emit("pieceFocus",getPlayerId(),pieceId);
        }

        event.stopPropagation();
      });
    }
});
 
//Attach drop event to all chessboard squares on click
const squares = document.querySelectorAll('.box');
squares.forEach(square => {
    square.addEventListener('click', function(){
      // let OpponentColor=(getPlayerId() == ID.PLAYER1 ? "B" : "W");

      // if (clickedPiece!=null &&  (this.children.length === 0 || this.firstChild.getAttribute('src')[7]==OpponentColor)  )
      // {
      //   // If the square is empty or opponent piece, only then append the clicked piece

      //   //TODO: Emit move request  to server
      //   let oldPosition = clickedPiece.parentNode.id;
      //   let newPosition = this.id;
      //   let pieceId= clickedPiece.getAttribute('pieceid');

      //   console.log(getPlayerId(),pieceId,oldPosition,newPosition);

      //   socket.emit("pieceMove",getPlayerId(),pieceId,objPosition(oldPosition),objPosition(newPosition));
      //   clickedPiece.style.border = '';
      //   clickedPiece=null
      // }

      if (clickedPiece!=null)
      {
        // If the square is empty or opponent piece, only then append the clicked piece

        //TODO: Emit move request  to server
        let oldPosition = clickedPiece.parentNode.id;
        let newPosition = this.id;
        let pieceId= clickedPiece.getAttribute('pieceid');

        console.log(getPlayerId(),pieceId,oldPosition,newPosition);
        console.log("played drop");

        socket.emit("pieceMove",getPlayerId(),pieceId,objPosition(oldPosition),objPosition(newPosition));
        clickedPiece.style.border = '';
        clickedPiece=null
        unHighlightCells();
      } 
    });
});
  
}
//Adds message to chatbox
function addMessage(message, playerId) {
  // Create a new list item for the message
  let newMessage = document.createElement('li');
  let alignment = (getPlayerId()==playerId ? "right":"left");
  newMessage.className = 'message ' + alignment;
  
  // Create a paragraph element to hold the message text
  let messageText = document.createElement('p');
  messageText.textContent = message;
  
  // Append the message text to the new list item
  newMessage.appendChild(messageText);
  
  // Find the chat container and add the new message to it
  let chatContainer = document.querySelector('.chat');
  chatContainer.insertBefore(newMessage, chatContainer.firstChild);
}

function printMove(playerId,pieceId,newPosition,pieceKilled)
{
  let moveTable = document.querySelector(".tab");

  if(playerId==ID.PLAYER1)
  {
  let newEntry = document.createElement("tr");
  let indexCell = document.createElement("td");
  indexCell.textContent = moveTable.children.length; 
  newEntry.appendChild(indexCell);

  let user1Cell = document.createElement("td");
  user1Cell.textContent = moveChessNotation(pieceId,newPosition,pieceKilled);
  newEntry.appendChild(user1Cell);

  let user2Cell = document.createElement("td");
  user2Cell.textContent = "  ";//empty for now
  newEntry.appendChild(user2Cell);

  // Append the new row to the table body
  moveTable.appendChild(newEntry);
  }
  else
  { //for Black
    let entry= moveTable.lastChild.lastChild;
    entry.textContent = moveChessNotation(pieceId,newPosition,pieceKilled);
  }
}

//Calculates the Moves notation for printing
//Assuming newPosition as string eg.: "24" or "58"
function moveChessNotation(pieceId,newPosition,pieceKilled)
{
  let rowNum=newPosition[0];//string
  let colNum=newPosition[1];//string
  let colAlpha= String.fromCharCode(97 - 1 + eval(colNum));
  let newPosStr=colAlpha+rowNum
  let pieceKilledChar=(pieceKilled==true?"x":"")
  let pieceChar="";

  switch(true)
  {
    case pieceId.startsWith("bishop"):
        pieceChar = "B";
        break;
    case pieceId.startsWith("knight"):
        pieceChar = "N";
        break;
    case pieceId.startsWith("rook"):
        pieceChar = "R";
        break;
    case pieceId.startsWith("queen"):
        pieceChar = "Q";
        break;
    case pieceId.startsWith("king"):
        pieceChar = "K";
        break;
    default:
        // Default empty eg. pawn
        pieceChar = ""; 
        break;
  }

  return pieceChar+pieceKilledChar+newPosStr;
}
