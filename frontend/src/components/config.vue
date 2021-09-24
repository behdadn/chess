<template>
  <div id="config">
    <button onClick="window.location.reload();" id="restart">restart</button>
    <button v-on:click="switchColor()">play as {{ oppColor }}</button>
    <button v-on:click="theme()">
      {{ this.$store.getters.siteTheme }} mode
    </button>
    <h3>difficulty: {{ this.$store.state.difficulty.title }}</h3>
    <h3>change:</h3>
    <ul>
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
        { number: 0, title: "random" },
        { number: 1, title: "easy" },
        { number: 2, title: "beginner" },
        { number: 3, title: "intermediate" },
        { number: 4, title: "advanced" },
        { number: 5, title: "experienced" },
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
  list-style-type: none;
  margin: 0;
  padding: 0;
}
#restart {
  width: 150px;
  height: 100px;
  font-size: 36px;
}
button {
  width: 100px;
  height: 60px;
}
</style>
