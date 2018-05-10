<template>
    <div>
        <div class="buttons">
            <button v-on:click="$emit('deal')">Deal</button>
        </div>
        <div v-if="points" class="points">
            <div>
                AI Points: <span>{{points[0]}}</span>
            </div>
            <div>
                Your Points: <span>{{points[1]}}</span>
            </div>
            <div>
                Cards remaining: <span>{{stack.length}}</span>
            </div>
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
div.buttons {
    display: inline-block;
}
div.points {
    display: inline-block;
}
div.points div {
    padding: 0 10px;
    display: inline-block;
}
</style>