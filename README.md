# Algorithmic chess engine

The code uses the [THC chess library](https://github.com/billforsternz/thc-chess-library) in order to facilitate movement of the pieces and board visualization in the backend. For the frontend, the website is built using Vue.js, and the actual chessboard on the website is rendered using [vue-chessboard](https://github.com/vitogit/vue-chessboard).

Different difficulties are the different depths that the algorithm calculates, so higher difficulties can be extremely inefficient. I plan to add alpha-beta pruning functionality to hopefully speed it up.
