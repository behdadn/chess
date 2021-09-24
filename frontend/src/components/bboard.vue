<script>
import { chessboard } from "vue-chessboard";
const jsChessEngine = require("js-chess-engine");

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
        this.aiMove();
      };
    },
    aiMove() {
      console.log("ai playing");
      const g = new jsChessEngine.Game(this.game.fen());
      let move = g.aiMove(this.$store.state.difficulty.number);
      this.game.move({
        from: Object.keys(move)[0].toLowerCase(),
        to: move[Object.keys(move)[0]].toLowerCase(),
      });
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
      movable: { events: { after: this.aiMove() } },
    });
  },
};
</script>
