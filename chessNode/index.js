function getPlayerId()
{
    return eval(sessionStorage.getItem("playerID"));
}

function buildChessBoard()
{
  let playerId=getPlayerId();

  if(playerId==0)
  {
    for(let row=8;row>=1;row--)
    {
      buildLogic(row);
    }
  }
  else
  {  
    for(let row=1;row<=8;row++)
    {
      buildLogic(row);
    }
  }

  insertChessPiece(playerId);

  function buildLogic(row)
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
      boxList.id = `${row}${i}`;
      rowDiv.appendChild(boxList);
    }
    let boardUl=document.querySelector('ul');
    boardUl.appendChild(rowDiv);
  }
}

//inserting the images
function insertChessPiece(playerId) {
  document.querySelectorAll(".box").forEach((square) => {
    
    square.style.cursor = "pointer";

    let rowNum=eval(square.id[0]);
    if(rowNum==1 ||rowNum==2 ||rowNum==7||rowNum==8)
    {
      let imageName="";
      
      if(rowNum ==2 )
        imageName="Wpawn";
      else if( rowNum == 7)
        imageName="Bpawn";

      else if(square.id =="11" || square.id =="18")
        imageName="Wrook";
      else if(square.id =="12" || square.id =="17")
        imageName="Wknight";
      else if(square.id =="13" || square.id =="16")
        imageName="Wbishop";
      

      else if(square.id =="81" || square.id =="88")
        imageName="Brook";
      else if(square.id =="82" || square.id =="87")
        imageName="Bknight";
      else if(square.id =="83" || square.id =="86")
        imageName="Bbishop";
      

      if(playerId==0)
      {
        if(square.id =="14")
          imageName="Wqueen";
        else if(square.id =="15")
          imageName="Wking";
        else if(square.id =="84")
          imageName="Bqueen";
        else if(square.id =="85")
          imageName="Bking";
      }
      else
      {
        if(square.id =="14")
          imageName="Wking";
        else if(square.id =="15")
          imageName="Wqueen";
        else if(square.id =="84")
          imageName="Bking";
        else if(square.id =="85")
          imageName="Bqueen";
      }
      
      square.innerHTML = `<img class='all-img all-pown' src='images/${imageName}.png' draggable='true' alt=''>`;
      
    }
  });
}


// Function to apply selected theme
function coloring(theme,playerId) {
  const boxes = document.querySelectorAll('.box');
  boxes.forEach((box, index) => {
    const getId = box.id;
    const arr = Array.from(getId);
    const aside = eval(arr.pop());
    const aup = eval(arr.shift());
    const a = aside + aup;

        if ((a+playerId) % 2 == 0) 
        {
          box.classList.remove('default-mode1', 'dark-mode1', 'light-mode1', 'red-mode1', 'blue-mode1', 'green-mode1','purple-mode1','yellow-mode1','magenta-mode1','orange-mode1');
          box.classList.add(theme +'-mode1');
        } 
        else 
        {
          box.classList.remove('dark-mode', 'light-mode', 'red-mode', 'blue-mode', 'green-mode','purple-mode','yellow-mode','magenta-mode','orange-mode');
          box.classList.add(theme + '-mode');
        }
  });
}

//////////////////////Main logic from here//////////////////////////////
buildChessBoard();

//Coloring the board
document.addEventListener('DOMContentLoaded', function() {
  const themeSelect = document.getElementById('theme-select-board');

  // Event listener for theme change
  themeSelect.addEventListener('change', function() {
    const selectedTheme = themeSelect.value;
    coloring(selectedTheme,getPlayerId());
  });

});


const themeSelect = document.getElementById('theme-select-board').value;
coloring(themeSelect,getPlayerId());

//for drag handling
document.addEventListener('DOMContentLoaded', function() {
  const pieces = document.querySelectorAll('.box img');
  let draggedPiece = null;

  // Attach drag events to all chess pieces
  pieces.forEach(piece => {
      piece.addEventListener('dragstart', dragStart);
      piece.addEventListener('dragend', dragEnd);     
  });



  function dragStart(e) {
      draggedPiece = this;
      draggedPiece.style.border = '2px solid red';
      e.dataTransfer.setData('text/plain', ''); // Required for Firefox
  }

  function dragEnd(e) {
      draggedPiece.style.border = '';
      draggedPiece = null;
      
  }

  // Attach drop event to all chessboard squares
  const squares = document.querySelectorAll('.box');
  squares.forEach(square => {
      square.addEventListener('dragover', dragOver);
      square.addEventListener('drop', drop);
  });

  function dragOver(e) {
      e.preventDefault();
  }

  function drop() {
      if (draggedPiece) {
        if (this.children.length === 0){
          // If the square is empty, only then append the dragged piece
          this.appendChild(draggedPiece);
        } 
      }
  }
});

//for click handling
document.addEventListener('DOMContentLoaded', function() {
  const pieces = document.querySelectorAll('.box img');
  let clickedPiece=null;
  pieces.forEach(piece => {

    piece.addEventListener('click', function() {
      if (this.style.border === '2px solid red') {
        // If already highlighted, remove the highlight
        this.style.border = '';
        clickedPiece = null;
      }
      else if(clickedPiece!=null)
      {
        clickedPiece.style = '';
        //assign curr as clicked piece
        this.style.border = '2px solid red';
        clickedPiece = this;
      } 
      else {
        // If not highlighted, apply the highlight
        this.style.border = '2px solid red';
        clickedPiece = this;
    }
    });

    const squares = document.querySelectorAll('.box');
    squares.forEach(square => {
        square.addEventListener('click', function(){
          if (this.children.length === 0 && clickedPiece!=null){
            // If the square is empty, only then append the clicked piece
            this.appendChild(clickedPiece);
            clickedPiece.style.border = '';
            clickedPiece=null
          } 
        });
    });

});
 
  
});

const socket = io();
socket.on('serverEmit', (msg) => {
  console.log("msg from server in client:",msg);
});



// function coloring() {
//   const color = document.querySelectorAll(".box");

//   color.forEach((color) => {
//     getId = color.id;
//     arr = Array.from(getId);
//     arr.shift();
//     aside = eval(arr.pop());
//     aup = eval(arr.shift());
//     a = aside + aup;

//     if (a % 2 == 0) {
//       color.style.backgroundColor = "rgb(32 235 239)";
//     }
//     if (a % 2 !== 0) {
//       color.style.backgroundColor = "rgb(40 180 200)";
//     }
//   });
// }
// coloring();

//function to not remove the same team element

// function reddish() {
//   document.querySelectorAll(".box").forEach((i1) => {
//     if (i1.style.backgroundColor == "blue") {
//       document.querySelectorAll(".box").forEach((i2) => {
//         if (
//           i2.style.backgroundColor == "greenyellow" &&
//           i2.innerText.length !== 0
//         ) {
//           greenyellowText = i2.innerText;

//           blueText = i1.innerText;

//           blueColor = Array.from(blueText).shift().toString();
//           greenyellowColor = Array.from(greenyellowText).shift().toString();

//           getId = i2.id;
//           arr = Array.from(getId);
//           arr.shift();
//           aside = eval(arr.pop());
//           aup = eval(arr.shift());
//           a = aside + aup;

//           if (a % 2 == 0 && blueColor == greenyellowColor) {
//             i2.style.backgroundColor = "rgb(32 235 239)";
//           }
//           if (a % 2 !== 0 && blueColor == greenyellowColor) {
//             i2.style.backgroundColor = "rgb(25 135 150)";
//           }
//         }
//       });
//     }
//   });
// }

// //reset button
// document.getElementById("reset-btn").addEventListener("click", function () {
//   location.reload();
// });

// tog = 1;

// document.querySelectorAll(".box").forEach((item) => {
//   item.addEventListener("click", function () {
//     if (
//       item.style.backgroundColor == "greenyellow" &&
//       item.innerText.length == 0
//     ) {
//       tog = tog + 1;
//     } else if (
//       item.style.backgroundColor == "greenyellow" &&
//       item.innerText.length !== 0
//     ) {
//       document.querySelectorAll(".box").forEach((i) => {
//         if (i.style.backgroundColor == "blue") {
//           blueId = i.id;
//           blueText = i.innerText;

//           document.getElementById(blueId).innerText = "";
//           item.innerText = blueText;
//           coloring();
//           insertImage();
//           tog = tog + 1;
//         }
//       });
//     }

//     getId = item.id;
//     console.log(getId);//
//     arr = Array.from(getId);
//     console.log(arr);//
//     arr.shift();
//     console.log(arr);//
//     aside = eval(arr.pop());
//     console.log(aside);//
//     arr.push("0");
//     console.log(arr);//
//     aup = eval(arr.join(""));
//     console.log(aup);//
//     a = aside + aup;
//     console.log(a);//

//     //function to display the available paths for all pieces

//     function whosTurn(toggle) {
//       // PAWN

//       if (item.innerText == `${toggle}pawn`) {
//         item.style.backgroundColor = "blue";

//         if (tog % 2 !== 0 && aup < 800) {
//           // First move for white pawns
//           if (document.getElementById(`b${a + 100}`).innerText.length == 0) {
//             document.getElementById(`b${a + 100}`).style.backgroundColor =
//               "greenyellow";
//             if (
//               document.getElementById(`b${a + 200}`).innerText.length == 0 &&
//               aup < 300
//             ) {
//               document.getElementById(`b${a + 200}`).style.backgroundColor =
//                 "greenyellow";
//             }
//           }
//           if (
//             aside < 8 &&
//             document.getElementById(`b${a + 100 + 1}`).innerText.length !== 0
//           ) {
//             document.getElementById(`b${a + 100 + 1}`).style.backgroundColor =
//               "greenyellow";
//           }
//           if (
//             aside > 1 &&
//             document.getElementById(`b${a + 100 - 1}`).innerText.length !== 0
//           ) {
//             document.getElementById(`b${a + 100 - 1}`).style.backgroundColor =
//               "greenyellow";
//           }
//         }

//         if (tog % 2 == 0 && aup > 100) {
//           // First move for black pawns
//           if (document.getElementById(`b${a - 100}`).innerText.length == 0) {
//             document.getElementById(`b${a - 100}`).style.backgroundColor =
//               "greenyellow";
//             if (
//               document.getElementById(`b${a - 200}`).innerText.length == 0 &&
//               aup > 600
//             ) {
//               document.getElementById(`b${a - 200}`).style.backgroundColor =
//                 "greenyellow";
//             }
//           }
//           if (
//             aside < 8 &&
//             document.getElementById(`b${a - 100 + 1}`).innerText.length !== 0
//           ) {
//             document.getElementById(`b${a - 100 + 1}`).style.backgroundColor =
//               "greenyellow";
//           }
//           if (
//             aside > 1 &&
//             document.getElementById(`b${a - 100 - 1}`).innerText.length !== 0
//           ) {
//             document.getElementById(`b${a - 100 - 1}`).style.backgroundColor =
//               "greenyellow";
//           }
//         }
//         // Second move for pawns
//         if (tog % 2 !== 0 && aup >= 800) {
//           if (document.getElementById(`b${a + 100}`).innerText.length == 0) {
//             document.getElementById(`b${a + 100}`).style.backgroundColor =
//               "greenyellow";
//           }
//           if (
//             aside < 8 &&
//             document.getElementById(`b${a + 100 + 1}`).innerText.length !== 0
//           ) {
//             document.getElementById(`b${a + 100 + 1}`).style.backgroundColor =
//               "greenyellow";
//           }
//           if (
//             aside > 1 &&
//             document.getElementById(`b${a + 100 - 1}`).innerText.length !== 0
//           ) {
//             document.getElementById(`b${a + 100 - 1}`).style.backgroundColor =
//               "greenyellow";
//           }
//         }
//         if (tog % 2 == 0 && aup <= 100) {
//           if (document.getElementById(`b${a - 100}`).innerText.length == 0) {
//             document.getElementById(`b${a - 100}`).style.backgroundColor =
//               "greenyellow";
//           }
//           if (
//             aside < 8 &&
//             document.getElementById(`b${a - 100 + 1}`).innerText.length !== 0
//           ) {
//             document.getElementById(`b${a - 100 + 1}`).style.backgroundColor =
//               "greenyellow";
//           }
//           if (
//             aside > 1 &&
//             document.getElementById(`b${a - 100 - 1}`).innerText.length !== 0
//           ) {
//             document.getElementById(`b${a - 100 - 1}`).style.backgroundColor =
//               "greenyellow";
//           }
//         }
//       }

//       // KING

//       if (item.innerText == `${toggle}king`) {
//         if (aside < 8) {
//           document.getElementById(`b${a + 1}`).style.backgroundColor =
//             "greenyellow";
//         }
//         if (aside > 1) {
//           document.getElementById(`b${a - 1}`).style.backgroundColor =
//             "greenyellow";
//         }
//         if (aup < 800) {
//           document.getElementById(`b${a + 100}`).style.backgroundColor =
//             "greenyellow";
//         }
//         if (aup > 100) {
//           document.getElementById(`b${a - 100}`).style.backgroundColor =
//             "greenyellow";
//         }

//         if (aup > 100 && aside < 8) {
//           document.getElementById(`b${a - 100 + 1}`).style.backgroundColor =
//             "greenyellow";
//         }
//         if (aup > 100 && aside > 1) {
//           document.getElementById(`b${a - 100 - 1}`).style.backgroundColor =
//             "greenyellow";
//         }
//         if (aup < 800 && aside < 8) {
//           document.getElementById(`b${a + 100 + 1}`).style.backgroundColor =
//             "greenyellow";
//         }
//         if (aup < 800 && aside > 1) {
//           document.getElementById(`b${a + 100 - 1}`).style.backgroundColor =
//             "greenyellow";
//         }

//         item.style.backgroundColor = "blue";
//       }

//       // KNIGHT

//       if (item.innerText == `${toggle}knight`) {
//         if (aside < 7 && aup < 800) {
//           document.getElementById(`b${a + 100 + 2}`).style.backgroundColor =
//             "greenyellow";
//         }
//         if (aside < 7 && aup > 200) {
//           document.getElementById(`b${a - 100 + 2}`).style.backgroundColor =
//             "greenyellow";
//         }
//         if (aside < 8 && aup < 700) {
//           document.getElementById(`b${a + 200 + 1}`).style.backgroundColor =
//             "greenyellow";
//         }
//         if (aside > 1 && aup < 700) {
//           document.getElementById(`b${a + 200 - 1}`).style.backgroundColor =
//             "greenyellow";
//         }
//         if (aside > 2 && aup < 800) {
//           document.getElementById(`b${a - 2 + 100}`).style.backgroundColor =
//             "greenyellow";
//         }
//         if (aside > 2 && aup > 100) {
//           document.getElementById(`b${a - 2 - 100}`).style.backgroundColor =
//             "greenyellow";
//         }
//         if (aside < 8 && aup > 200) {
//           document.getElementById(`b${a - 200 + 1}`).style.backgroundColor =
//             "greenyellow";
//         }
//         if (aside > 1 && aup > 200) {
//           document.getElementById(`b${a - 200 - 1}`).style.backgroundColor =
//             "greenyellow";
//         }

//         item.style.backgroundColor = "blue";
//       }

//       // QUEEN

//       if (item.innerText == `${toggle}queen`) {
//         for (let i = 1; i < 9; i++) {
//           if (
//             a + i * 100 < 900 &&
//             document.getElementById(`b${a + i * 100}`).innerText == 0
//           ) {
//             document.getElementById(`b${a + i * 100}`).style.backgroundColor =
//               "greenyellow";
//           } else if (
//             a + i * 100 < 900 &&
//             document.getElementById(`b${a + i * 100}`).innerText !== 0
//           ) {
//             document.getElementById(`b${a + i * 100}`).style.backgroundColor =
//               "greenyellow";
//             break;
//           }
//         }

//         for (let i = 1; i < 9; i++) {
//           if (
//             a - i * 100 > 100 &&
//             document.getElementById(`b${a - i * 100}`).innerText == 0
//           ) {
//             document.getElementById(`b${a - i * 100}`).style.backgroundColor =
//               "greenyellow";
//           } else if (
//             a - i * 100 > 100 &&
//             document.getElementById(`b${a - i * 100}`).innerText !== 0
//           ) {
//             document.getElementById(`b${a - i * 100}`).style.backgroundColor =
//               "greenyellow";
//             break;
//           }
//         }

//         for (let i = 1; i < 9; i++) {
//           if (
//             a + i < aup + 9 &&
//             document.getElementById(`b${a + i}`).innerText == 0
//           ) {
//             document.getElementById(`b${a + i}`).style.backgroundColor =
//               "greenyellow";
//           } else if (
//             a + i < aup + 9 &&
//             document.getElementById(`b${a + i}`).innerText !== 0
//           ) {
//             document.getElementById(`b${a + i}`).style.backgroundColor =
//               "greenyellow";
//             break;
//           }
//         }

//         for (let i = 1; i < 9; i++) {
//           if (
//             a - i > aup &&
//             document.getElementById(`b${a - i}`).innerText == 0
//           ) {
//             document.getElementById(`b${a - i}`).style.backgroundColor =
//               "greenyellow";
//           } else if (
//             a - i > aup &&
//             document.getElementById(`b${a - i}`).innerText !== 0
//           ) {
//             document.getElementById(`b${a - i}`).style.backgroundColor =
//               "greenyellow";
//             break;
//           }
//         }

//         for (let i = 1; i < 9; i++) {
//           if (
//             i < (900 - aup) / 100 &&
//             i < 9 - aside &&
//             document.getElementById(`b${a + i * 100 + i}`).innerText.length == 0
//           ) {
//             document.getElementById(
//               `b${a + i * 100 + i}`
//             ).style.backgroundColor = "greenyellow";
//           } else if (
//             i < (900 - aup) / 100 &&
//             i < 9 - aside &&
//             document.getElementById(`b${a + i * 100 + i}`).innerText.length !==
//               0
//           ) {
//             document.getElementById(
//               `b${a + i * 100 + i}`
//             ).style.backgroundColor = "greenyellow";
//             break;
//           }
//         }

//         for (let i = 1; i < 9; i++) {
//           if (
//             i < aup / 100 &&
//             i < 9 - aside &&
//             document.getElementById(`b${a - i * 100 + i}`).innerText.length == 0
//           ) {
//             document.getElementById(
//               `b${a - i * 100 + i}`
//             ).style.backgroundColor = "greenyellow";
//           } else if (
//             i < aup / 100 &&
//             i < 9 - aside &&
//             document.getElementById(`b${a - i * 100 + i}`).innerText.length !==
//               0
//           ) {
//             document.getElementById(
//               `b${a - i * 100 + i}`
//             ).style.backgroundColor = "greenyellow";
//             break;
//           }
//         }

//         for (let i = 1; i < 9; i++) {
//           if (
//             i < (900 - aup) / 100 &&
//             i < aside &&
//             document.getElementById(`b${a + i * 100 - i}`).innerText.length == 0
//           ) {
//             document.getElementById(
//               `b${a + i * 100 - i}`
//             ).style.backgroundColor = "greenyellow";
//           } else if (
//             i < (900 - aup) / 100 &&
//             i < aside &&
//             document.getElementById(`b${a + i * 100 - i}`).innerText.length !==
//               0
//           ) {
//             document.getElementById(
//               `b${a + i * 100 - i}`
//             ).style.backgroundColor = "greenyellow";
//             break;
//           }
//         }

//         for (let i = 1; i < 9; i++) {
//           if (
//             i < aup / 100 &&
//             i < aside &&
//             document.getElementById(`b${a - i * 100 - i}`).innerText.length == 0
//           ) {
//             document.getElementById(
//               `b${a - i * 100 - i}`
//             ).style.backgroundColor = "greenyellow";
//           } else if (
//             i < aup / 100 &&
//             i < aside &&
//             document.getElementById(`b${a - i * 100 - i}`).innerText.length !==
//               0
//           ) {
//             document.getElementById(
//               `b${a - i * 100 - i}`
//             ).style.backgroundColor = "greenyellow";
//             break;
//           }
//         }

//         item.style.backgroundColor = "blue";
//       }

//       // BISHOP

//       if (item.innerText == `${toggle}bishop`) {
//         for (let i = 1; i < 9; i++) {
//           if (
//             i < (900 - aup) / 100 &&
//             i < 9 - aside &&
//             document.getElementById(`b${a + i * 100 + i}`).innerText.length == 0
//           ) {
//             document.getElementById(
//               `b${a + i * 100 + i}`
//             ).style.backgroundColor = "greenyellow";
//           } else if (
//             i < (900 - aup) / 100 &&
//             i < 9 - aside &&
//             document.getElementById(`b${a + i * 100 + i}`).innerText.length !==
//               0
//           ) {
//             document.getElementById(
//               `b${a + i * 100 + i}`
//             ).style.backgroundColor = "greenyellow";
//             break;
//           }
//         }

//         for (let i = 1; i < 9; i++) {
//           if (
//             i < aup / 100 &&
//             i < 9 - aside &&
//             document.getElementById(`b${a - i * 100 + i}`).innerText.length == 0
//           ) {
//             document.getElementById(
//               `b${a - i * 100 + i}`
//             ).style.backgroundColor = "greenyellow";
//           } else if (
//             i < aup / 100 &&
//             i < 9 - aside &&
//             document.getElementById(`b${a - i * 100 + i}`).innerText.length !==
//               0
//           ) {
//             document.getElementById(
//               `b${a - i * 100 + i}`
//             ).style.backgroundColor = "greenyellow";
//             break;
//           }
//         }

//         for (let i = 1; i < 9; i++) {
//           if (
//             i < (900 - aup) / 100 &&
//             i < aside &&
//             document.getElementById(`b${a + i * 100 - i}`).innerText.length == 0
//           ) {
//             document.getElementById(
//               `b${a + i * 100 - i}`
//             ).style.backgroundColor = "greenyellow";
//           } else if (
//             i < (900 - aup) / 100 &&
//             i < aside &&
//             document.getElementById(`b${a + i * 100 - i}`).innerText.length !==
//               0
//           ) {
//             document.getElementById(
//               `b${a + i * 100 - i}`
//             ).style.backgroundColor = "greenyellow";
//             break;
//           }
//         }

//         for (let i = 1; i < 9; i++) {
//           if (
//             i < aup / 100 &&
//             i < aside &&
//             document.getElementById(`b${a - i * 100 - i}`).innerText.length == 0
//           ) {
//             document.getElementById(
//               `b${a - i * 100 - i}`
//             ).style.backgroundColor = "greenyellow";
//           } else if (
//             i < aup / 100 &&
//             i < aside &&
//             document.getElementById(`b${a - i * 100 - i}`).innerText.length !==
//               0
//           ) {
//             document.getElementById(
//               `b${a - i * 100 - i}`
//             ).style.backgroundColor = "greenyellow";
//             break;
//           }
//         }

//         item.style.backgroundColor = "blue";
//       }

//       // ROOK

//       if (item.innerText == `${toggle}rook`) {
//         for (let i = 1; i < 9; i++) {
//           if (
//             a + i * 100 < 900 &&
//             document.getElementById(`b${a + i * 100}`).innerText == 0
//           ) {
//             document.getElementById(`b${a + i * 100}`).style.backgroundColor =
//               "greenyellow";
//           } else if (
//             a + i * 100 < 900 &&
//             document.getElementById(`b${a + i * 100}`).innerText !== 0
//           ) {
//             document.getElementById(`b${a + i * 100}`).style.backgroundColor =
//               "greenyellow";
//             break;
//           }
//         }

//         for (let i = 1; i < 9; i++) {
//           if (
//             a - i * 100 > 100 &&
//             document.getElementById(`b${a - i * 100}`).innerText == 0
//           ) {
//             document.getElementById(`b${a - i * 100}`).style.backgroundColor =
//               "greenyellow";
//           } else if (
//             a - i * 100 > 100 &&
//             document.getElementById(`b${a - i * 100}`).innerText !== 0
//           ) {
//             document.getElementById(`b${a - i * 100}`).style.backgroundColor =
//               "greenyellow";
//             break;
//           }
//         }

//         for (let i = 1; i < 9; i++) {
//           if (
//             a + i < aup + 9 &&
//             document.getElementById(`b${a + i}`).innerText == 0
//           ) {
//             document.getElementById(`b${a + i}`).style.backgroundColor =
//               "greenyellow";
//           } else if (
//             a + i < aup + 9 &&
//             document.getElementById(`b${a + i}`).innerText !== 0
//           ) {
//             document.getElementById(`b${a + i}`).style.backgroundColor =
//               "greenyellow";
//             break;
//           }
//         }

//         for (let i = 1; i < 9; i++) {
//           if (
//             a - i > aup &&
//             document.getElementById(`b${a - i}`).innerText == 0
//           ) {
//             document.getElementById(`b${a - i}`).style.backgroundColor =
//               "greenyellow";
//           } else if (
//             a - i > aup &&
//             document.getElementById(`b${a - i}`).innerText !== 0
//           ) {
//             document.getElementById(`b${a - i}`).style.backgroundColor =
//               "greenyellow";
//             break;
//           }
//         }

//         item.style.backgroundColor = "blue";
//       }
//     }

//     // Toggling the turn

//     if (tog % 2 !== 0) {
//       document.getElementById("tog").innerText = "White's Turn";
//       whosTurn("W");
//     }
//     if (tog % 2 == 0) {
//       document.getElementById("tog").innerText = "Black's Turn";
//       whosTurn("B");
//     }

//     reddish();
//   });
// });

// // Moving the element
// document.querySelectorAll(".box").forEach((hathiTest) => {
//   hathiTest.addEventListener("click", function () {
//     if (hathiTest.style.backgroundColor == "blue") {
//       blueId = hathiTest.id;
//       blueText = hathiTest.innerText;

//       document.querySelectorAll(".box").forEach((hathiTest2) => {
//         hathiTest2.addEventListener("click", function () {
//           if (
//             hathiTest2.style.backgroundColor == "greenyellow" &&
//             hathiTest2.innerText.length == 0
//           ) {
//             document.getElementById(blueId).innerText = "";
//             hathiTest2.innerText = blueText;
//             coloring();
//             insertImage();
//           }
//         });
//       });
//     }
//   });
// });

// // Prvents from selecting multiple elements
// z = 0;
// document.querySelectorAll(".box").forEach((ee) => {
//   ee.addEventListener("click", function () {
//     z = z + 1;
//     if (z % 2 == 0 && ee.style.backgroundColor !== "greenyellow") {
//       coloring();
//     }
//   });
// });

// document.addEventListener('DOMContentLoaded', function() {
//   const boxes = document.querySelectorAll('.box');

//   let draggedPiece = null;

//   // Attach drag events to all boxes
//   boxes.forEach(box => {
//       box.addEventListener('dragstart', dragStart);
//       box.addEventListener('dragenter', dragEnter);
//       box.addEventListener('dragover', dragOver);
//       box.addEventListener('dragleave', dragLeave);
//       box.addEventListener('drop', drop);
//       box.addEventListener('dragend', dragEnd);
//   });

//   function dragStart(e) {
//       draggedPiece = this;
//       e.dataTransfer.setData('text/plain', ''); // Required for Firefox
//   }

//   function dragEnter(e) {
//       e.preventDefault();
//       this.classList.add('highlight');
//   }

//   function dragOver(e) {
//       e.preventDefault();
//   }

//   function dragLeave() {
//       this.classList.remove('highlight');
//   }

//   function drop() {
//       this.appendChild(draggedPiece);
//       this.classList.remove('highlight');
//   }

//   function dragEnd() {
//       draggedPiece = null;
//   }
// });
