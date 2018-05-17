<template>
  <div v-if="$store.state.gameState" class="played-cards">
    <div class="card" v-bind:class="['card__' + $store.state.gameState.playedCards[0]]"></div>
    <div class="card" v-bind:class="['card__' + $store.state.gameState.playedCards[1]]"></div>
  </div>
</template>

<script>
export default {
  name: 'PlayedCards'
}
</script>

<style scoped>
div.played-cards div:first-child {
  margin-right: 10px;
}
</style>