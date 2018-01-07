<?php

namespace App\Http\Controllers;

use Illuminate\Http\Request;
use Kreait\Firebase\Factory;
use Kreait\Firebase\ServiceAccount;

class FirebaseController extends Controller
{
    public function update()
    {
        $firebase = (new Factory)
            ->withCredentials(base_path() . '/firebase.json')
            ->create();
        $database = $firebase->getDatabase();
        $reference = $database->getReference('/count');
        $oldCount = $reference->getValue();
        $oldCount++;
        $reference->set($oldCount);
        return redirect("/");
    }
}
