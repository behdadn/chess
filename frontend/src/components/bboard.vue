<script>
import { chessboard } from "vue-chessboard";

export default {
  name: "board",
  // extend from vue-chessboard library
  extends: chessboard,
  methods: {
    userPlay() {
      // method that runs when the user is making a move
      return (orig, dest) => {
        if (this.isPromotion(orig, dest)) {
          this.promoteTo = this.onPromotion();
        }
        // promote to queen automatically:
        this.game.move({ from: orig, to: dest, promotion: this.promoteTo });
        this.board.set({
          fen: this.game.fen(),
        });
        this.calculatePromotions();
        if (this.game.in_checkmate()) {
          // check if checkmate and sets game status
          this.$store.commit("setStatus", 1);
        }
        if (this.game.in_draw() || this.game.in_stalemate()) {
          // check if draw and sets game status
          this.$store.commit("setStatus", 3);
        }
        // runs the function that fetches the AI move
        this.getmove();
      };
    },

    getmove() {
      // method that fetches the move from the eval websocket server
      let socket = new WebSocket("ws://localhost:9002");
      // connects to the c++ websocket server

      var self = this;

      socket.onopen = function (event) {
        // runs when websocket connects
        console.log(event);
        socket.send(
          // sends current game fen and difficulty to the eval server
          (self.$store.state.difficulty.number + 1).toString() +
            " " +
            self.game.fen()
        );
      };

      socket.onmessage = function (event) {
        // passes move returned by the c++ server to the playmove function
        self.playmove(event.data);
      };
    },

    playmove(mv) {
      // method that actually plays the move returned by the websocket server
      this.game.move(mv);
      // makes the move passed to the method

      this.board.set({
        // sets up the board for the next user move
        fen: this.game.fen(),
        turnColor: this.toColor(),
        movable: {
          color: this.toColor(),
          dests: this.possibleMoves(),
          events: { after: this.userPlay() },
        },
      });
      if (this.game.in_checkmate()) {
        // check if checkmate and change game status
        this.$store.commit("setStatus", 2);
      }
      if (this.game.in_draw() || this.game.in_stalemate()) {
        // check if draw and change game status
        this.$store.commit("setStatus", 3);
      }
    },
  },
  mounted() {
    this.board.set({
      movable: { events: { after: this.getmove() } },
      // sets first method to run to computer move
    });
  },
};
</script>
