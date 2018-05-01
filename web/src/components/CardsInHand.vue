<template>
  <div>
    <button v-on:click="deal()">Deal</button>
    <div v-if="state" class="human-cards">
      <div v-bind:class="['card__' + state.cardsInHand[1][0]]"></div>
      <div v-bind:class="['card__' + state.cardsInHand[1][1]]"></div>
      <div v-bind:class="['card__' + state.cardsInHand[1][2]]"></div>
    </div>
  </div>
</template>

<script>
import axios from "axios";

export default {
  name: 'CardsInHand',
  data () {
    return {
      state: null
    }
  },
  methods: {
    deal () {
      axios({ method: "POST", "url": "http://localhost:5000/deal" }).then(result => {
        this.state = result.data;
        console.log(this.state)
      }, error => {
        console.error(error);
      });
    }
  }
}
</script>

<!-- Add "scoped" attribute to limit CSS to this component only -->
<style scoped>
div.human-cards div {
  background-size: cover;  
  display: inline-block;
  height: 240px;
  width: 120px;
}
div.human-cards div:first-child {
  margin-right: 10px;
}
div.human-cards div:last-child {
  margin-left: 10px;
}
div.human-cards div.card__0 {
  background-image: url('~../assets/cards/card__0.png');
}
div.human-cards div.card__1 {
  background-image: url('~../assets/cards/card__1.png');
}
div.human-cards div.card__2 {
  background-image: url('~../assets/cards/card__2.png');
}
div.human-cards div.card__3 {
  background-image: url('~../assets/cards/card__3.png');
}
div.human-cards div.card__4 {
  background-image: url('~../assets/cards/card__4.png');
}
div.human-cards div.card__5 {
  background-image: url('~../assets/cards/card__5.png');
}
div.human-cards div.card__6 {
  background-image: url('~../assets/cards/card__6.png');
}
div.human-cards div.card__7 {
  background-image: url('~../assets/cards/card__7.png');
}
div.human-cards div.card__8 {
  background-image: url('~../assets/cards/card__8.png');
}
div.human-cards div.card__9 {
  background-image: url('~../assets/cards/card__9.png');
}
div.human-cards div.card__10 {
  background-image: url('~../assets/cards/card__10.png');
}
div.human-cards div.card__11 {
  background-image: url('~../assets/cards/card__11.png');
}
div.human-cards div.card__12 {
  background-image: url('~../assets/cards/card__12.png');
}
div.human-cards div.card__13 {
  background-image: url('~../assets/cards/card__13.png');
}
div.human-cards div.card__14 {
  background-image: url('~../assets/cards/card__14.png');
}
div.human-cards div.card__15 {
  background-image: url('~../assets/cards/card__15.png');
}
div.human-cards div.card__16 {
  background-image: url('~../assets/cards/card__16.png');
}
div.human-cards div.card__17 {
  background-image: url('~../assets/cards/card__17.png');
}
div.human-cards div.card__18 {
  background-image: url('~../assets/cards/card__18.png');
}
div.human-cards div.card__19 {
  background-image: url('~../assets/cards/card__19.png');
}
div.human-cards div.card__20 {
  background-image: url('~../assets/cards/card__20.png');
}
div.human-cards div.card__21 {
  background-image: url('~../assets/cards/card__21.png');
}
div.human-cards div.card__22 {
  background-image: url('~../assets/cards/card__22.png');
}
div.human-cards div.card__23 {
  background-image: url('~../assets/cards/card__23.png');
}
div.human-cards div.card__24 {
  background-image: url('~../assets/cards/card__24.png');
}
div.human-cards div.card__25 {
  background-image: url('~../assets/cards/card__25.png');
}
div.human-cards div.card__26 {
  background-image: url('~../assets/cards/card__26.png');
}
div.human-cards div.card__27 {
  background-image: url('~../assets/cards/card__27.png');
}
div.human-cards div.card__28 {
  background-image: url('~../assets/cards/card__28.png');
}
div.human-cards div.card__29 {
  background-image: url('~../assets/cards/card__29.png');
}
div.human-cards div.card__30 {
  background-image: url('~../assets/cards/card__30.png');
}
div.human-cards div.card__31 {
  background-image: url('~../assets/cards/card__31.png');
}
div.human-cards div.card__32 {
  background-image: url('~../assets/cards/card__32.png');
}
div.human-cards div.card__33 {
  background-image: url('~../assets/cards/card__33.png');
}
div.human-cards div.card__34 {
  background-image: url('~../assets/cards/card__34.png');
}
div.human-cards div.card__35 {
  background-image: url('~../assets/cards/card__35.png');
}
div.human-cards div.card__36 {
  background-image: url('~../assets/cards/card__36.png');
}
div.human-cards div.card__37 {
  background-image: url('~../assets/cards/card__37.png');
}
div.human-cards div.card__38 {
  background-image: url('~../assets/cards/card__38.png');
}
div.human-cards div.card__39 {
  background-image: url('~../assets/cards/card__39.png');
}
</style>
