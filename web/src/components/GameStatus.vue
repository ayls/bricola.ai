<template>
    <div>
        <div class="game-status">
            <div class="buttons">
                <button v-on:click="deal()" class="button button-large button-gray">New Game</button>
            </div>
            <div v-if="$store.state.gameState" class="play-status">
                <div>
                    <span>{{formatMoveStatus()}}</span>
                </div>
            </div>
            <div v-if="$store.state.gameState" class="points">
                <div>
                    AI: <span>{{$store.state.gameState.points[0]}}</span>
                </div>
                <div>
                    You: <span>{{$store.state.gameState.points[1]}}</span>
                </div>
            </div>            
        </div>

        <Modal ref="dealModal" v-bind:buttons="['Yes', 'No']" v-on:close="closeDealModal($event)">
            <template slot="header">Start a new game?</template>
            <template slot="text">A game is in progress, are you sure you want to start a new game?</template>
        </Modal>

        <Modal ref="gameEndedModal" v-bind:buttons="['Yes', 'No']" v-on:close="closeEndOfGameModal($event)">
            <template slot="header">{{formatEndOfGameStatus()}}</template>
            <template slot="text">Do you want to start a new game?</template>
        </Modal>

        <Modal ref="loadingModal">
            <template slot="text">{{loadingMessage}}</template>
        </Modal>

        <Modal ref="errorModal" v-bind:buttons="['Ok']">
            <template slot="header">An Error Occured</template>
            <template slot="text">There was an error when communicating with server</template>
        </Modal>        
    </div>
</template>

<script>
import Modal from "./Modal.vue";

export default {
  name: "GameStatus",
  data () {
      return {
          loadingPromise: null,
          loadingMessage: null,
          loadingIntervalHandle: null,
          loadingMessageId: 0,
          loadingRetries: 0
      };
  },
  computed: {
    gameStatus() {
      return this.$store.state.gameState
        ? this.$store.state.gameState.status
        : null
    }
  },
  methods: {
    deal() {
      if (this.loadingPromise && !this.loadingPromise.done) {
        return;
      }

      if (
        this.$store.state.gameState &&
        this.gameStatus &&
        ["aiWon", "youWon", "tie"].indexOf(this.gameStatus) === -1
      ) {
        this.$refs.dealModal.open();
      } else {
        this._deal();
      }
    },
    formatMoveStatus() {
      if (this.gameStatus === "aiMove") {
        return "AI move";
      }

      if (this.gameStatus === "yourMove") {
        return "Your move";
      }

      return null;
    },
    formatEndOfGameStatus() {
      if (this.gameStatus === "aiWon") {
        return "AI Won!";
      }

      if (this.gameStatus === "youWon") {
        return "You Won!";
      }

      if (this.gameStatus === "tie") {
        return "It is a tie!";
      }

      return null;
    },
    closeDealModal(result) {
      if (result === "Yes") {
        this._deal();
      }
    },    
    closeEndOfGameModal(result) {
      if (result === "Yes") {
        this._deal();
      } else {
        this.$store.commit("set", null);
      }
    },
    _deal() {
      this.loadingPromise = this.$apiClient.deal();
      this.loadingPromise
        .then(result => {
          this.$store.commit("set", result);
          this.loadingPromise.done = true;
        })
        .catch(error => {
          if (this.loadingIntervalHandle && this.loadingRetries < 3) {
            this.loadingRetries++;
            clearInterval(this.loadingIntervalHandle);
            this._displayLoadingMessages();
          } else {
            this._closeLoadingModal();
            this.$refs.errorModal.open();
          }

          console.log(error);
        });

      setTimeout(() => {
        if (!this.loadingPromise.done) {
          this._displayLoadingMessages();
        }
      }, 1500);
    },    
    _displayLoadingMessages() {
      const messages = [
        "Oops, it appears the server is asleep...",
        "I am waking it up!",
        "Might take a while though",
        "I am running this on the cheap side, you know",
        "So bear with me, please",
        "Are we there yet? (obviously not)",
        "Eh, what about a cup of coffee?",
        "Jeeze, this is horribly slow",
        "Wake up already will ya!?",
        "Hold on I am going to check what is going on",
        "I'll be back, wait here!"
      ];
      this.loadingMessage = messages[this.loadingMessageId];
      this.$refs.loadingModal.open();
      this.loadingIntervalHandle = setInterval(() => {
        if (!this.loadingPromise.done) {
          this.loadingMessageId = this.loadingMessageId < messages.length - 1 ? this.loadingMessageId + 1 : this.loadingMessageId;
          this.loadingMessage = messages[this.loadingMessageId];
        } else {
          clearInterval(this.loadingIntervalHandle);
          this._closeLoadingModal();
        }
      }, 5000);
    },    
    _closeLoadingModal () {
      this.$refs.loadingModal.close();
      this.loadingMessageId = 0;
      this.loadingRetries = 0;
      this.loadingPromise = null;
    },
  },
  watch: {
    gameStatus: function(newStatus) {
      if (["aiWon", "youWon", "tie"].indexOf(newStatus) !== -1) {
        this.$refs.gameEndedModal.open();
      }
    }
  },
  components: {
    Modal
  }
};
</script>

<style scoped>
div.game-status {
  font-size: 45px;
  font-weight: bold;
  display: grid;
  grid-template-columns: repeat(3, 1fr);
}
div.game-status span {
  color: yellow;
}
div.buttons {
  grid-column: 1;
  text-align: left;
}
div.play-status {
  grid-column: 2;
}
div.play-status div {
  padding: 0 10px;
  position: relative;
  top: 50%;
  transform: translateY(-50%);
}
div.points {
  grid-column: 3;
  text-align: right;
}
div.points div {
  padding: 0 10px;
  display: inline-block;
  position: relative;
  top: 50%;
  transform: translateY(-50%);
}
</style>