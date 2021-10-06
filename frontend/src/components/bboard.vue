<script>
import { chessboard } from "vue-chessboard";

export default {
  name: "board",
  // inherit chessboard from library
  extends: chessboard,
  methods: {
    userPlay() {
      console.log("your turn");
      return (orig, dest) => {
        if (this.isPromotion(orig, dest)) {
          this.promoteTo = this.onPromotion();
        }
        this.game.move({ from: orig, to: dest, promotion: this.promoteTo }); // promote to queen for simplicity
        this.board.set({
          fen: this.game.fen(),
        });
        this.calculatePromotions();
        if (this.game.in_checkmate()) {
          //   this.$store.commit("gameWon");
          this.$store.commit("setStatus", 1);
        }
        if (this.game.in_draw() || this.game.in_stalemate()) {
          //   this.$store.commit("draw");
          this.$store.commit("setStatus", 3);
        }
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
      movable: { events: { after: this.getmove() } },
    });
  },
};
</script>
