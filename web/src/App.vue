<template>
  <div id="app">
    <CardsInHand/>
  </div>
</template>

<script>
import CardsInHand from './components/CardsInHand.vue'

export default {
  name: 'app',
  components: {
    CardsInHand
  }
}
</script>

<style>
#app {
  font-family: 'Avenir', Helvetica, Arial, sans-serif;
  -webkit-font-smoothing: antialiased;
  -moz-osx-font-smoothing: grayscale;
  text-align: center;
  color: #2c3e50;
  margin-top: 60px;
}
</style>
