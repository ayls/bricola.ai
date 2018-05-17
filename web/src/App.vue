<template>
  <div id="app">
    <div class="status">
      <game-status 
        v-on:deal="deal()"/>
    </div>
    <div class="content">
      <div class="board">
        <div class="left">
          <stack />
        </div>
        <div class="right">
          <div>
            <played-cards />
          </div>
        </div>
        <div class="bottom">
          <div class="bottom-part" style="display: none">
            <cards-in-hand v-bind:handIdx="0" />
          </div>
          <div class="bottom-part">
            <cards-in-hand v-bind:handIdx="1" v-on:play="play($event)"/>
          </div>
        </div>
      </div>
    </div>
    <div class="footer">
      <footer-links />
    </div>
  </div>
</template>

<script>
import GameStatus from "./components/GameStatus.vue";
import Stack from "./components/Stack.vue";
import PlayedCards from "./components/PlayedCards.vue";
import CardsInHand from "./components/CardsInHand.vue";
import FooterLinks from "./components/FooterLinks.vue";

export default {
  name: "App",
  props: ["baseUrl"],
  methods: {
    deal() {
      this.$apiClient
        .deal()
        .then(result => {
          this.$store.commit('set', result);
        })
        .catch(error => {
          console.log(error);
        });
    },
    play(cardIdx) {
      if (cardIdx != null) {
        this.$store.commit('setPlayedCard', cardIdx);
      }

      this._play();
    },
    _play() {
      this.$apiClient
        .play(this.$store.state.gameState)
        .then(result => {
          this.$store.commit('set', result);
          if (result.status === "aiMove") {
            setTimeout(() => {
              this._play();
            }, 500);
          } else if (result.status === "endOfTurn") {
            setTimeout(() => {
              this._play();
            }, 1500);
          }
        })
        .catch(error => {
          console.log(error);
        });
    }
  },
  components: {
    GameStatus,
    Stack,
    PlayedCards,
    CardsInHand,
    FooterLinks
  }
};
</script>

<style>
#app {
  font-family: "Avenir", Helvetica, Arial, sans-serif;
  -webkit-font-smoothing: antialiased;
  -moz-osx-font-smoothing: grayscale;
  text-align: center;
  padding: 0 20px;
  display: flex;
  flex-flow: column;
  height: 100%;
}
div.status {
  padding: 20px 0;
  border-bottom: 1px solid white;
  flex: 0 1 auto;
}
div.content {
  flex: 1 1 auto;
}
div.board {
  padding-top: 20px;
  padding-bottom: 20px;
  display: grid;
  grid-template-columns: repeat(2, 1fr);
}
div.board div.left {
  grid-column: 1;
  grid-row: 1;
}
div.board div.right {
  grid-column: 2;
  grid-row: 1;
}
div.board div.bottom {
  padding-top: 40px;
  grid-column: 1 / span 2;
  grid-row: 2;
}
div.board div.bottom .bottom-part:last-child {
  margin-top: 10px;
}
div.footer {
  border-top: 1px solid white;
  padding-top: 10px;
  flex: 0 1 60px;
}
</style>
