SimpleStepper
=============

<p>Simple library for driving Stepper motors with arduino. This library is for driving your stepper motor without the need of a step-direction type driver. A dual H-bridge and you're done.</p>

<h3>Features</h3>
<p>
  <ul>
    <li>Multiple turning units (steps, degrees, revolutions).</li>
    <li>Accelerated and decelerated movements to manage inertia.</li>
    <li>High and low speed supported.</li>
    <li>Rotor position tracking.</li>
    <li>Customizable parameters (max. velocity, acceleration, position).</li>
  </ul>
</p>
<h3>Limitations:</h>
<p>
  <ul>
    <li>The functions are blocking, i.e. you can't do anything else while the motor is moving.</li>
    <li>You can drive multiple motors but one at a time.</li>
    <li>For now, only Arduino UNO board is supported.</li>
    <li>Each motor needs 4 pins to be driven.</li>
  </ul>
</p>
<p>This is under developement and it's been made only for pedagogical reasons, and to eliminate the need for external drivers.</p>
