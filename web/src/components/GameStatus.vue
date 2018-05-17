<template>
    <div>
        <div class="game-status">
            <div class="buttons">
                <button v-on:click="deal()" class="button button-large button-gray">New Game</button>
            </div>
            <div v-if="points" class="points">
                <div>
                    AI: <span>{{points[0]}}</span>
                </div>
                <div>
                    You: <span>{{points[1]}}</span>
                </div>
            </div>
            <div v-if="status" class="play-status">
                <div>
                    <span>{{formatStatus(status)}}</span>
                </div>
            </div>
        </div>

        <Modal ref="dealModal" v-bind:buttons="['Yes', 'No']" v-on:close="closeModal($event)">
            <template slot="header">Start a new game?</template>
            <template slot="text">A game is in progress, are you sure you want to start a new game?</template>
        </Modal>
    </div>
</template>

<script>
import Modal from './Modal.vue';

export default {
  name: 'GameStatus',
  props: [
    'points',
    'status'
  ],
  methods: {
    deal () {
        if (this.status && ['aiWon', 'youWon', 'tie'].indexOf(this.status) === -1) {
            this.$refs.dealModal.open();
        } else {
            this.$emit('deal');
        }
    },
    closeModal(result) {
        if (result === 'Yes') {
            this.$emit('deal');
        }
    },
    formatStatus (status) {
        if (status === 'aiMove') {
            return 'AI move';
        }

        if (status === 'yourMove') {
            return 'Your move';
        }

        if (status === 'aiWon') {
            return 'AI Won!';
        }

        if (status === 'youWon') {
            return 'You Won!';
        }

        if (status === 'tie') {
            return 'It is a tie!';
        }

        return null;
    }
  },
  components: {
    Modal 
  }  
}
</script>

<style scoped>
div.game-status {
    font-size: 45px;
    font-weight: bold;
    display: grid;
    grid-template-columns: repeat(7, 1fr);
}
div.game-status span {
    color: yellow;
}
div.buttons {
    grid-column: 1 / span 2;
    text-align: left;
}
div.points {
    grid-column: 3 / span 3;
}
div.points div {
    padding: 0 10px;
    display: inline-block;    
    position: relative;
    top: 50%;
    transform: translateY(-50%);    
}
div.play-status {
    grid-column: 6 / span 2;
    text-align: right;
}
div.play-status div {
    padding: 0 10px;
    position: relative;
    top: 50%;
    transform: translateY(-50%);       
}
</style>