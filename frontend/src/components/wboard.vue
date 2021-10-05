<script>
import { chessboard } from "vue-chessboard";
const jsChessEngine = require("js-chess-engine");
const fetch = require("node-fetch");

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
        if (this.$store.state.difficulty.number === 0) {
          // if the difficulty is random, play random move
          this.random();
        } else {
          // ai move
          this.wsAI();
        }
      };
    },

    async random() {
      // connect to API and fetch best move:
      let link = "http://localhost:8000/fen/";
      link += this.fenParse(this.game.fen());
      const response = await fetch(link);
      const move = await response.text();
      let movefrom = move.substring(0, 2).toLowerCase().trim();
      let moveto = move.substring(2).toLowerCase().trim();

      this.game.move({
        from: movefrom,
        to: moveto,
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

    wsAI() {
      let webSocket = new WebSocket("ws://localhost:9002");

      webSocket.onopen = function (event) {
        webSocket.send(
          this.$store.state.difficulty.number.toString() + this.game.fen()
        );
      };

      let move;

      webSocket.onmessage = function (event) {
        move = event.data;
      };

      this.game.move({
        to: move,
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

    aiMove() {
      const g = new jsChessEngine.Game(this.game.fen());
      let move = g.aiMove(this.$store.state.difficulty.number - 1);
      console.log(Object.keys(move)[0].toLowerCase());
      console.log(typeof Object.keys(move)[0].toLowerCase());

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

    fenParse(fen1) {
      let fen = "";
      for (let i = 0; i < fen1.length; i++) {
        if (fen1[i] === "/" || fen1[i] === " ") {
          fen += "-";
        } else if (fen1[i] === "-") {
          fen += "x";
        } else {
          fen += fen1[i];
        }
      }
      return fen;
    },
  },
  mounted() {
    this.board.set({
      movable: { events: { after: this.userPlay() } },
    });
  },
};
</script>
