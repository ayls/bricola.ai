<template>
    <div class="game-status">
        <div class="buttons">
            <button v-on:click="$emit('deal')" class="button button-large">Deal</button>
        </div>
        <div v-if="points && stack" class="points">
            <div>
                AI Points: <span>{{points[0]}}</span>
            </div>
            <div>
                Your Points: <span>{{points[1]}}</span>
            </div>
            <div>
                Cards remaining: <span>{{stack.length}}</span>
            </div>
        </div>
        <div v-if="status" class="play-status">
            <div>
                <span>{{formatStatus(status)}}</span>
            </div>
        </div>
    </div>
</template>

<script>
export default {
  name: 'GameStatus',
  props: [
    'points',
    'stack',
    'status'
  ],
  methods: {
    formatStatus (status) {
      if (status === 'aiMove') {
        return 'AI move';
      }

      if (status === 'yourMove') {
        return 'Your move';
      }

      if (status === 'endOfTurn') {
        return 'End of turn';
      }

      if (status === 'gameOver') {
        return 'Game over';
      }

      return status;
    }
  }  
}
</script>

<style scoped>
div {
    font-size: 45px;
    font-weight: bold;
    display: inline-block;
}
div span {
    color: yellow;
}
div.game-status {
    display: grid;
    grid-template-columns: repeat(5, 1fr);
}
div.buttons {
    grid-column: 1;
    text-align: left;
}
div.points {
    grid-column: 2 / span 3;
}
div.points div {
    padding: 0 10px;
}
div.play-status {
    grid-column: 5;
    text-align: right;
}
div.play-status div {
    padding: 0 10px;
}
</style>