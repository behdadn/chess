<template>
  <div id="app" v-bind:style="this.$store.getters.siteColor">
    <a href="."><h1>chess</h1></a>
    <div id="game">
      <div id="wboard" v-if="this.$store.state.userColor === 'white'">
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
        <config v-bind:oppColor="this.$store.getters.otherColor" />
      </div>
    </div>
    <h1>{{ this.$store.state.gameStatus }}</h1>
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
  padding-right: 50px;
}
#bboard {
  padding-right: 50px;
}
#game {
  display: flex;
  flex-direction: row;
  align-items: center;
}
#config {
  width: 200px;
}
a {
  color: inherit; /* blue colors for links too */
}
</style>
