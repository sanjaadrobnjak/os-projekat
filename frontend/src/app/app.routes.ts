import { Routes } from '@angular/router';
import { RegisterComponent } from './register/register.component';
import { LoginComponent } from './login/login.component';
import { HelperComponent } from './helper/helper.component';
import { ProvideComponent } from './provide/provide.component';
import { SeniorComponent } from './senior/senior.component';
import { ProfileComponent } from './profile/profile.component';
import { RequestComponent } from './request/request.component';
import { SettingsComponent } from './settings/settings.component';
import { ViewserviceComponent } from './viewservice/viewservice.component';

export const routes: Routes = [
    {path: "register", component: RegisterComponent},
    {path: "login", component: LoginComponent},
    {path: "helper", component: HelperComponent},
    {path: "provide", component: ProvideComponent},
    {path: "senior", component: SeniorComponent},
    {path: "viewProfile", component: ProfileComponent},
    {path: "request", component: RequestComponent},
    {path: "settings", component: SettingsComponent},
    {path: "viewservice", component: ViewserviceComponent}
];
