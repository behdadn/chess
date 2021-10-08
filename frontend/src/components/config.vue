<template>
  <div id="config">
    <button onClick="window.location.reload();" id="restart">restart</button>
    <!-- used to reload the site if the user wants to restart the game -->
    <button v-on:click="switchColor()">play as {{ oppColor }}</button>
    <!-- button that switches the side that the user is playing as -->
    <button v-on:click="theme()">
      <!-- button that changes the theme of the website when the user wants to -->
      {{ this.$store.getters.siteTheme }} mode
    </button>
    <h3>difficulty: {{ this.$store.state.difficulty.title }}</h3>
    <!-- shows the difficulty of the AI that the user is playing against -->
    <h3>change:</h3>
    <ul>
      <!-- generates a button for every difficulty so the user can use them to change -->
      <li v-for="di in difs" :key="di.title">
        <button v-on:click="dif(di.number)">{{ di.title }}</button>
      </li>
    </ul>
  </div>
</template>

<script>
export default {
  name: "config",
  props: {
    oppColor: String,
  },
  data: function () {
    return {
      // easy access to difficulties
      difs: [
        { number: 0, title: "very easy" },
        { number: 1, title: "beginner" },
        { number: 2, title: "intermediate" },
        { number: 3, title: "advanced" },
        { number: 4, title: "experienced" },
      ],
    };
  },
  methods: {
    // switch color
    switchColor() {
      this.$store.commit("changeColor");
    },
    // change difficulty
    dif(n) {
      this.$store.commit("setDif", n);
    },
    // change theme
    theme() {
      this.$store.commit("switchTheme");
    },
  },
};
</script>

<style scoped>
ul {
  /* css for the difficulty buttons */
  list-style-type: none;
  margin: 0;
  padding: 0;
}
#restart {
  /* css for the restart button */
  width: 150px;
  height: 100px;
  font-size: 36px;
}
button {
  /* css for all the buttons */
  width: 100px;
  height: 60px;
}
</style>
