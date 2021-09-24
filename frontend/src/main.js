import Vue from "vue";
import Vuex from "vuex";
import App from "./App.vue";

Vue.use(Vuex);
Vue.config.productionTip = false;

const difref = [
  { 0: "random" },
  { 1: "easy" },
  { 2: "beginner" },
  { 3: "intermediate" },
  { 4: "advanced" },
  { 5: "experienced" },
];

const difficulty = 0;

const store = new Vuex.Store({
  // set up vuex store
  state: {
    // assign state variables
    reset: 0,
    gameStatus: "ongoing",
    userColor: "white",
    difficulty: {
      number: difficulty,
      title: difref[difficulty][difficulty],
    },
    siteTheme: {
      theme: "dark",
      color: "background: #2A2B2E; color: #61E786",
    },
  },
  getters: {
    // getters for the state variables
    otherColor: (state) => {
      if (state.userColor == "white") {
        return "black";
      } else {
        return "white";
      }
    },
    siteColor: (state) => {
      return state.siteTheme.color;
    },
    siteTheme: (state) => {
      if (state.siteTheme.theme === "light") {
        return "dark";
      } else {
        return "light";
      }
    },
  },
  mutations: {
    // functions that change the state variable
    switchTheme(state) {
      if (state.siteTheme.theme === "light") {
        state.siteTheme.theme = "dark";
        state.siteTheme.color = "background: #2A2B2E; color: #61E786";
      } else {
        state.siteTheme.theme = "light";
        state.siteTheme.color = "background: #7f98a3; color: #2c3e50;";
      }
    },
    setStatus(state, n) {
      switch (n) {
        case 0:
          state.gameStatus = "ongoing";
          break;
        case 1:
          state.gameStatus = "won";
          break;
        case 2:
          state.gameStatus = "lost";
          break;
        case 3:
          state.gameStatus = "draw";
          break;
      }
    },
    gameWon(state) {
      state.gameStatus = "won";
    },
    gameLost(state) {
      state.gameStatus = "lost";
    },
    draw(state) {
      state.gameStatus = "draw";
    },
    changeColor(state) {
      if (state.userColor === "white") {
        state.userColor = "black";
      } else {
        state.userColor = "white";
      }
    },
    setDif(state, n) {
      state.difficulty.number = n;
      state.difficulty.title = difref[n][n];
      console.log(state.difficulty.number);
    },
    restart(state) {
      state.reset++;
    },
  },
});

new Vue({
  render: (h) => h(App),
  store: store,
}).$mount("#app");
