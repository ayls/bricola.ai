<template>
  <div v-if="stack && stack.length > 0">
      <div class="card card-overlap card__100"></div>
      <div class="card card-rotate" v-bind:class="['card__' + briscola]"></div>
  </div>
</template>

<script>
export default {
  name: "Stack",
  props: [
    'stack',
    'briscola'
  ]
}
</script>

<style scoped>
div.card-rotate {
    -webkit-transform: rotate(90deg);
    -moz-transform: rotate(90deg);
    -ms-transform: rotate(90deg);
    -o-transform: rotate(90deg);
    transform: rotate(90deg);
}
div.card-overlap {
    z-index: 100;
    position: absolute;
}
</style>