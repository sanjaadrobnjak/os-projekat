import { Component } from '@angular/core';
import { UserService } from '../services/user.service';
import { Router } from '@angular/router';
import { FormsModule } from '@angular/forms';

@Component({
  selector: 'app-login',
  standalone: true,
  imports: [FormsModule],
  templateUrl: './login.component.html',
  styleUrl: './login.component.css'
})
export class LoginComponent {
  constructor(private userService: UserService, private router : Router) {}
  username: string = "";
  password: string = "";
  error = ""

  login(){
    if (this.username == "") {
      this.error = "Please enter username!";
    }
    else if (this.password == "") {
      this.error = "Please enter password!";
    }
    else {
      this.userService.login(this.username, this.password).subscribe(data => {
          if (data == null) {
            this.error = 'This user does not exist!';
          } else {
            alert("Uspesno se prijavio")
            this.router.navigate([`/${data.type}`], {
              state: {user: data}
            });
          }
        });
    }
  }

}
