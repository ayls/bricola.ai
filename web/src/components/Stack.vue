<template>
  <div v-if="$store.state.gameState && $store.state.gameState.stack.length > 0">
      <div class="card card-overlap card__100"><div><span>{{$store.state.gameState.stack.length}}</span></div></div>
      <div class="card card-rotate" v-bind:class="['card__' + $store.state.gameState.briscola]"></div>
  </div>
</template>

<script>
export default {
  name: 'Stack'
}
</script>

<style scoped>
div.card-rotate {
    -webkit-transform: rotate(90deg);
    -moz-transform: rotate(90deg);
    -ms-transform: rotate(90deg);
    -o-transform: rotate(90deg);
    transform: rotate(90deg);
    margin: 0 0 0 140px;
}
div.card-overlap {
    z-index: 100;
    position: absolute;
}
div.card-overlap div {
    position: relative;
    top: 50%;
    transform: translateY(-50%);
}
div.card-overlap div span {
    font-size: 45px;
    font-weight: bold;
    color: yellow;
}
</style>