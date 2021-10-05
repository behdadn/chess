<script>
import { chessboard } from "vue-chessboard";

export default {
  name: "wboard",
  // extend from vue-chessboard library
  extends: chessboard,
  methods: {
    userPlay() {
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
          // check if checkmate
          this.$store.commit("setStatus", 1);
        }
        if (this.game.in_draw() || this.game.in_stalemate()) {
          // check if draw
          this.$store.commit("setStatus", 3);
        }
        // if (this.$store.state.difficulty.number === 0) {
        //   // if the difficulty is random, play random move
        //   this.random();
        // }
        // ai move
        this.getmove();
      };
    },

    getmove() {
      let socket = new WebSocket("ws://localhost:9002");

      var self = this;

      socket.onopen = function (event) {
        console.log(event);
        socket.send(
          (self.$store.state.difficulty.number + 1).toString() +
            " " +
            self.game.fen()
        );
      };

      socket.onmessage = function (event) {
        self.playmove(event.data);
      };
    },

    playmove(mv) {
      this.game.move(mv);

      this.board.set({
        fen: this.game.fen(),
        turnColor: this.toColor(),
        movable: {
          color: this.toColor(),
          dests: this.possibleMoves(),
          events: { after: this.userPlay() },
        },
      });
      if (this.game.in_checkmate()) {
        // this.$store.commit("gameLost");
        this.$store.commit("setStatus", 2);
      }
      if (this.game.in_draw() || this.game.in_stalemate()) {
        // this.$store.commit("draw");
        this.$store.commit("setStatus", 3);
      }
    },
  },
  mounted() {
    this.board.set({
      movable: { events: { after: this.userPlay() } },
    });
  },
};
</script>
