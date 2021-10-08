import Vue from "vue";
import Vuex from "vuex";
import App from "./App.vue";

Vue.use(Vuex);
Vue.config.productionTip = false;

// store names of the difficulties for later use
const difref = [
  { 0: "very easy" },
  { 1: "beginner" },
  { 2: "intermediate" },
  { 3: "advanced" },
  { 4: "experienced" },
];

// sets default difficulty to beginner
const difficulty = 1;

const store = new Vuex.Store({
  // set up vuex store: used to store variables that are used by multiple components
  state: {
    // assign state variables
    gameStatus: "ongoing", // used to tell if either side won the game
    userColor: "white", // stores what color the player is playing as
    difficulty: {
      // stores currect difficulty
      number: difficulty,
      title: difref[difficulty][difficulty],
    },
    siteTheme: {
      // stores current site theme; default is dark
      theme: "dark",
      color: "background: #2A2B2E; color: #61E786",
    },
  },
  getters: {
    // getters for the state variables
    otherColor: (state) => {
      // returns the color of the computer player
      if (state.userColor == "white") {
        return "black";
      } else {
        return "white";
      }
    },
    siteColor: (state) => {
      // returns the CSS of the website currectly
      return state.siteTheme.color;
    },
    siteTheme: (state) => {
      // returns the theme that is not currently enabled to use with the button
      if (state.siteTheme.theme === "light") {
        return "dark";
      } else {
        return "light";
      }
    },
  },
  mutations: {
    // functions that change the state variables
    switchTheme(state) {
      // switches site theme
      if (state.siteTheme.theme === "light") {
        // checks if current theme is light to switch to dark
        state.siteTheme.theme = "dark";
        state.siteTheme.color = "background: #2A2B2E; color: #61E786";
      } else {
        // checks if current theme is dark to switch to light
        state.siteTheme.theme = "light";
        state.siteTheme.color = "background: #7f98a3; color: #2c3e50;";
      }
    },
    setStatus(state, n) {
      // changes game status depending on what is currently happening in the game
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
      // used to change the color that the user is playing as
      if (state.userColor === "white") {
        state.userColor = "black";
      } else {
        state.userColor = "white";
      }
    },
    setDif(state, n) {
      // sets the depth of the eval function
      state.difficulty.number = n;
      state.difficulty.title = difref[n][n];
      console.log(state.difficulty.number);
    },
  },
});

new Vue({
  render: (h) => h(App),
  store: store,
}).$mount("#app");
