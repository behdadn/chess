<template>
  <div id="app" v-bind:style="this.$store.getters.siteColor">
    <!-- sets the css of the page depending on the theme the user chose -->
    <a href="."><h1>chess</h1></a>
    <div id="game">
      <div id="wboard" v-if="this.$store.state.userColor === 'white'">
        <!-- sets the board up depending on which side the user wants to play as -->
        <wboard
          v-bind:orientation="this.$store.state.userColor"
          key="this.$store.state.reset"
        />
      </div>
      <div id="bboard" v-else>
        <bboard
          v-bind:orientation="this.$store.state.userColor"
          key="this.$store.state.reset"
        />
      </div>
      <div id="config">
        <!-- adds the config component to the site -->
        <config v-bind:oppColor="this.$store.getters.otherColor" />
      </div>
    </div>
    <h1>{{ this.$store.state.gameStatus }}</h1>
    <!-- prints out the status of the game (win, loss, draw, ongoing) -->
  </div>
</template>

<script>
// import components:
import wboard from "./components/wboard.vue";
import bboard from "./components/bboard.vue";
import config from "./components/config.vue";
// import chessboard style:
import "vue-chessboard/dist/vue-chessboard.css";

// export components to template:
export default {
  name: "App",
  components: {
    wboard,
    bboard,
    config,
  },
};
</script>

<style>
#app {
  /* style for the page as a whole */
  padding: 0px;
  margin: 0px;
  font-family: Avenir, Helvetica, Arial, sans-serif;
  -webkit-font-smoothing: antialiased;
  -moz-osx-font-smoothing: grayscale;
  text-align: center;
  display: flex;
  flex-direction: column;
  align-items: center;
  position: absolute;
  top: 0px;
  right: 0px;
  bottom: 0px;
  left: 0px;
  height: 100vh;
}
#wboard {
  /* style for the boards */
  padding-right: 50px;
}
#bboard {
  padding-right: 50px;
}
#game {
  /* style for the div containing the board and config components */
  display: flex;
  flex-direction: row;
  align-items: center;
}
#config {
  /* style for config component */
  width: 200px;
}
a {
  color: inherit; /* blue colors for links too */
}
</style>
